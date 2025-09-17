// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

public: // components
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* camComp;

public: // inputs
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* imc_tps;

public: // 총
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* sniperComp;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerBaseComponent* playerMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerBaseComponent* playerFire;
};











