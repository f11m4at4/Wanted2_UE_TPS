// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;

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
	}}


// Called when the game starts
void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	movement = me->GetCharacterMovement();

	// 이동속도 초기화
	movement->MaxWalkSpeed = walkSpeed;
}


// Called every frame
void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 회전의 공간 변환
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::SetInputBinding(class UEnhancedInputComponent* playerInput)
{
	Super::SetInputBinding(playerInput);

	playerInput->BindAction(ia_move, ETriggerEvent::Triggered, this, &UPlayerMove::MoveInput);

	playerInput->BindAction(ia_run, ETriggerEvent::Started, this, &UPlayerMove::RunInput);
	playerInput->BindAction(ia_run, ETriggerEvent::Completed, this, &UPlayerMove::RunInput);

	// 회전
	playerInput->BindAction(ia_turn, ETriggerEvent::Triggered, this, &UPlayerMove::TurnInput);
	playerInput->BindAction(ia_lookUp, ETriggerEvent::Triggered, this, &UPlayerMove::LookUpInput);

	// 점프
	playerInput->BindAction(ia_jump, ETriggerEvent::Started, this, &UPlayerMove::JumpInput);
}


void UPlayerMove::MoveInput(const struct FInputActionValue& value)
{
	FVector2d v = value.Get<FVector2d>();
	direction.X = v.X;
	direction.Y = v.Y;
}

void UPlayerMove::RunInput(const struct FInputActionValue& value)
{
	bool isPressed = value.Get<bool>();
	if (isPressed)
	{
		movement->MaxWalkSpeed = runSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
}


void UPlayerMove::TurnInput(const struct FInputActionValue& value)
{
	float v = value.Get<float>();
	me->AddControllerYawInput(v);
}

void UPlayerMove::LookUpInput(const struct FInputActionValue& value)
{
	float v = value.Get<float>();
	me->AddControllerPitchInput(v);
}



void UPlayerMove::JumpInput(const struct FInputActionValue& value)
{
	me->Jump();
}