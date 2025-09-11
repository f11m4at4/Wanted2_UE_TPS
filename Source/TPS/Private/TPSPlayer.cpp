// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "EnemyFSM.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerAnim.h"
#include "TPS.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "InputAction.h"
#include "InputMappingContext.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 스켈레탈메시 데이터를 로드하고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	// 2. 데이터 로드가 잘 됐다면,
	if (TempMesh.Succeeded())
	{
		//  -> 데이터를 스켈레탈메시 컴포넌트에 할당하고 싶다.
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 3. 위치 설정, 회전 설정 하고 싶다.
		GetMesh()->SetRelativeLocation(FVector(0.000000,0.000000,-87.000000));
		GetMesh()->SetRelativeRotation(FRotator(0.000000,-90,0.000000));

		// Anim Blueprint Assign
		// static ConstructorHelpers::FClassFinder<UPlayerAnim> TempAnimBP(TEXT("'/Game/Blueprints/ABP_Player.ABP_Player_C'"));
		// if (TempAnimBP.Succeeded())
		// {
		// 	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		// 	GetMesh()->SetAnimInstanceClass(TempAnimBP.Class);
		// }
	}

	// SringArm component 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(GetCapsuleComponent());
	springArmComp->SetRelativeLocation(FVector(0, 0, 60));
	
	// Camera
	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	camComp->SetupAttachment(springArmComp);

	JumpMaxCount = 2;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGun(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGun.Object);
		gunMeshComp->SetRelativeLocation(FVector(-3.744689,6.193350,-0.488459));
		gunMeshComp->SetRelativeRotation(FRotator(20.881624,95.330790,-2.488091));
	}

	sniperComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperComp"));
	sniperComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGun(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper11.sniper11'"));
	if (TempSniperGun.Succeeded())
	{
		sniperComp->SetStaticMesh(TempSniperGun.Object);
		sniperComp->SetRelativeLocation(FVector(-32.582719,-2.353971,4.157812));
		sniperComp->SetRelativeRotation(FRotator(19.983734,99.443404,-2.149859));
		sniperComp->SetRelativeScale3D(FVector(0.14f));
	}

	// 총알효과
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (TempEffect.Succeeded())
	{
		bulletEffect = TempEffect.Object;
	}
	
	// 발사 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase> TempFireSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if (TempFireSound.Succeeded())
	{
		fireSound = TempFireSound.Object;
	}

	// UI
	static ConstructorHelpers::FClassFinder<UUserWidget> tempCrosshair(TEXT("'/Game/UI/WBP_CrosshairUI.WBP_CrosshairUI_C'"));
	if (tempCrosshair.Succeeded())
	{
		crosshairUIFactory = tempCrosshair.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> tempSniperUI(TEXT("'/Game/UI/WBP_SniperUI.WBP_SniperUI_C'"));
	if (tempSniperUI.Succeeded())
	{
		sniperUIFactory = tempSniperUI.Class;
	}

	//  카메라셰이크
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> tempCS(TEXT("'/Game/Blueprints/BP_FireCameraShake.BP_FireCameraShake_C'"));
	if (tempCS.Succeeded())
	{
		fireCameraShake = tempCS.Class;
	}


	// /////////////////////////// 입력 ///////////////////////////
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("InputMappingContext'/Game/TPSInput/IMC_TPS.IMC_TPS'"));
	if (TempIMC.Succeeded())
	{
		imc_tps = TempIMC.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputMove(TEXT("'/Game/TPSInput/IA_TPSMove.IA_TPSMove'"));
	if (TempInputMove.Succeeded())
	{
		ia_move = TempInputMove.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputRun(TEXT("'/Game/TPSInput/IA_TPSRun.IA_TPSRun'"));
	if (TempInputRun.Succeeded())
	{
		ia_run = TempInputRun.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputTurn(TEXT("'/Game/TPSInput/IA_TPSTurn.IA_TPSTurn'"));
	if (TempInputTurn.Succeeded())
	{
		ia_turn = TempInputTurn.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputLook(TEXT("'/Game/TPSInput/IA_TPSLookup.IA_TPSLookup'"));
	if (TempInputLook.Succeeded())
	{
		ia_lookUp = TempInputLook.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputJump(TEXT("'/Game/TPSInput/IA_TPSJump.IA_TPSJump'"));
	if (TempInputJump.Succeeded())
	{
		ia_jump = TempInputJump.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputFire(TEXT("'/Game/TPSInput/IA_TPSFire.IA_TPSFire'"));
	if (TempInputFire.Succeeded())
	{
		ia_fire = TempInputFire.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputGrenadeGun(TEXT("'/Game/TPSInput/IA_TPSGrenadeGun.IA_TPSGrenadeGun'"));
	if (TempInputGrenadeGun.Succeeded())
	{
		ia_grenadeGun = TempInputGrenadeGun.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputSniperGun(TEXT("'/Game/TPSInput/IA_TPSSniperGun.IA_TPSSniperGun'"));
	if (TempInputSniperGun.Succeeded())
	{
		ia_sniperGun = TempInputSniperGun.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputSniper(TEXT("'/Game/TPSInput/IA_TPSSniper.IA_TPSSniper'"));
	if (TempInputSniper.Succeeded())
	{
		ia_sniperMode = TempInputSniper.Object;
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// input mapping context 할당
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys)
		{
			subsys->AddMappingContext(imc_tps, 0);
		}
	}

	// 오브젝트 풀 만들기
	for (int i=0;i<bulletPoolSize;i++)
	{
		// 총알 만들기
		auto bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory);
		// 탄창에 넣기
		bulletPool.Add(bullet);
		// 총알 비활성화
		bullet->SetActive(false);
	}


	// UI 생성
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// crosshair
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	crosshairUI->AddToViewport();
	
	// sniper 활성화
	ChangeToSniperGun(FInputActionValue());

	// 이동속도 초기화
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 회전의 공간 변환
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	
	// 이동하고싶다.
	// FVector p0 = GetActorLocation();
	// FVector vt = direction * speed * DeltaTime;
	// FVector p = p0 + vt;
	// SetActorLocation(p, true);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput)
	{
		playerInput->BindAction(ia_move, ETriggerEvent::Triggered, this, &ATPSPlayer::MoveInput);
		
		playerInput->BindAction(ia_run, ETriggerEvent::Started, this, &ATPSPlayer::RunInput);
		playerInput->BindAction(ia_run, ETriggerEvent::Completed, this, &ATPSPlayer::RunInput);

		// 회전
		playerInput->BindAction(ia_turn, ETriggerEvent::Triggered, this, &ATPSPlayer::TurnInput);
		playerInput->BindAction(ia_lookUp, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUpInput);

		// 점프
		playerInput->BindAction(ia_jump, ETriggerEvent::Started, this, &ATPSPlayer::JumpInput);
		// 총쏘기
		playerInput->BindAction(ia_fire, ETriggerEvent::Started, this, &ATPSPlayer::FireInput);

		// 총바꾸기
		playerInput->BindAction(ia_grenadeGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToGrenadeGun);
		playerInput->BindAction(ia_sniperGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToSniperGun);

		// 스나이퍼모드 전환
		playerInput->BindAction(ia_sniperMode, ETriggerEvent::Started, this, &ATPSPlayer::SniperModeInput);
		playerInput->BindAction(ia_sniperMode, ETriggerEvent::Completed, this, &ATPSPlayer::SniperModeInput);

	}
}

void ATPSPlayer::MoveInput(const struct FInputActionValue& value)
{
	FVector2d v = value.Get<FVector2d>();
	direction.X = v.X;
	direction.Y = v.Y;
}

void ATPSPlayer::RunInput(const struct FInputActionValue& value)
{
	bool isPressed = value.Get<bool>();
	if (isPressed)
	{
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
}

void ATPSPlayer::TurnInput(const struct FInputActionValue& value)
{
	float v = value.Get<float>();
	AddControllerYawInput(v);
}

void ATPSPlayer::LookUpInput(const struct FInputActionValue& value)
{
	float v = value.Get<float>();
	AddControllerPitchInput(v);
}

void ATPSPlayer::JumpInput(const struct FInputActionValue& value)
{
	Jump();
}

void ATPSPlayer::FireInput(const struct FInputActionValue& value)
{
	// 총발사 애니메이션 재생
	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	if (anim)
	{
		anim->PlayAttackAnimation();
	}

	// 카메라 셰이크 재생
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(fireCameraShake);

	// 총발사 사운드 재생
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	
	if (bUsingGrenade)
	{
		// 총알 만들어서 발사시키기
		auto firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));

		// 탄창에서 가져와서 총알 발사하기
		if (bulletPool.Num() > 0)
		{
			auto bullet = bulletPool[0];
			bullet->SetActorTransform(firePosition);
			bullet->SetActive(true);
			// 탄창에서 총알 제거하기
			bulletPool.RemoveAt(0);
		}
	}
	else
	{
		// 스나이퍼건 총발사
		// 총발사해서 부딪힌 지점에 파편을 튀게 해보자
		// 필요정보 : 시작점, 끝점.
		FVector startPos = camComp->GetComponentLocation();
		FVector endPos = startPos + camComp->GetForwardVector() * 10000;

		// 부딪힌정보 저장할 변수
		FHitResult hitInfo;

		FCollisionQueryParams param;
		param.AddIgnoredActor(this);
		// 가상의 선을 쏜다.
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);

		if (bHit)
		{
			// 총알 파편효과 재생
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffect, hitInfo.Location);

			// 부딪힌녀석이 Simulate Physics 가 활성화 되어 있으면
			if (hitInfo.GetComponent()->IsSimulatingPhysics())
			{
				// -> 날려보내자
				hitInfo.GetComponent()->AddImpulseAtLocation( camComp->GetForwardVector() * 1000000, hitInfo.Location);
			}

			// 맞은 녀석이 enemy 면 피격 콜백 호출
			// auto enemy = hitInfo.GetActor()->GetComponentByClass(UEnemyFSM::StaticClass());
			auto enemy = Cast<UEnemyFSM>(hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM")));
			if (enemy)
			{
				enemy->OnDamageProcess(camComp->GetForwardVector());
			}
		}
	}
}

void ATPSPlayer::ChangeToGrenadeGun(const struct FInputActionValue& value)
{
	bUsingGrenade = true;
	gunMeshComp->SetVisibility(true);
	sniperComp->SetVisibility(false);
}

void ATPSPlayer::ChangeToSniperGun(const struct FInputActionValue& value)
{
	bUsingGrenade = false;
	gunMeshComp->SetVisibility(false);
	sniperComp->SetVisibility(true);
}

void ATPSPlayer::SniperModeInput(const struct FInputActionValue& value)
{
	// 유탄총이면 동작하지 않는다.
	if (bUsingGrenade)
	{
		return;
	}

	bool bInput = value.Get<bool>();
	if (bInput)
	{
		sniperUI->AddToViewport();
		camComp->SetFieldOfView(45);
		crosshairUI->RemoveFromParent();
	}
	else
	{
		sniperUI->RemoveFromParent();
		camComp->SetFieldOfView(90);
		crosshairUI->AddToViewport();
	}
}
