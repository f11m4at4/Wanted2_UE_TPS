// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TPS.h"


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
