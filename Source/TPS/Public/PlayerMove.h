// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

	virtual void SetInputBinding(class UEnhancedInputComponent* playerInput) override;

public:
	// 이동
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_move;
	
	FVector direction = FVector::ZeroVector;

	// 이동처리
	void MoveInput(const struct FInputActionValue& value);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_run;

	UPROPERTY()
	class UCharacterMovementComponent* movement;

	// 방향, 속도
	UPROPERTY(EditAnywhere, Category=Settings)
	float walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category=Settings)
	float runSpeed = 600;
	
	void RunInput(const struct FInputActionValue& value);


	////////////// 회전
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_turn;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_lookUp;

	
	void TurnInput(const struct FInputActionValue& value);
	void LookUpInput(const struct FInputActionValue& value);

	///////////// 점프 //////////////
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_jump;
	
	void JumpInput(const struct FInputActionValue& value);
};
