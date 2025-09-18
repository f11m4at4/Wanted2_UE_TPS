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
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PlayerFire.h"
#include "PlayerMove.h"


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

	// /////////////////////////// 입력 ///////////////////////////
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("InputMappingContext'/Game/TPSInput/IMC_TPS.IMC_TPS'"));
	if (TempIMC.Succeeded())
	{
		imc_tps = TempIMC.Object;
	}

	// PlayerMove
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	// playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
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
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput)
	{
		onInputBindingDelegate.Broadcast(playerInput);
		// playerMove->SetInputBinding(playerInput);
		// playerFire->SetInputBinding(playerInput);
	}
}

void ATPSPlayer::OnHitEvent()
{
	PRINTLOG(TEXT("Damaged !!!!"));
	hp--;
	MakeHPtoPercent();
	if (hp <= 0)
	{
		PRINTLOG(TEXT("Die!!!!! OTL..."));
	}
}

void ATPSPlayer::MakeHPtoPercent_Implementation()
{
	percent = hp / maxHP;
}
