// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TPS.h"
#include "Blueprint/UserWidget.h"


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
	gunMeshComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGun(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGun.Object);
		gunMeshComp->SetRelativeLocation(FVector(-10.000000,0.000000,110.000000));
	}

	sniperComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperComp"));
	sniperComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGun(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper11.sniper11'"));
	if (TempSniperGun.Succeeded())
	{
		sniperComp->SetStaticMesh(TempSniperGun.Object);
		sniperComp->SetRelativeLocation(FVector(-10.000000,20.000000, 110.000000));
		sniperComp->SetRelativeScale3D(FVector(0.2f));
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
	
	// sniper 활성화
	ChangeToSniperGun(FInputActionValue());
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
	}
	else
	{
		sniperUI->RemoveFromParent();
		camComp->SetFieldOfView(90);
	}
}
