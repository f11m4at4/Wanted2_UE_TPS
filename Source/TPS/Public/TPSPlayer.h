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
	
	// 이동
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_move;
	
	// 방향, 속도
	UPROPERTY(EditAnywhere, Category=Settings)
	float speed = 500;
	FVector direction = FVector::ZeroVector;
	
	// 이동처리
	void MoveInput(const struct FInputActionValue& value);

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

public: // 총
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* sniperComp;

	// 총쏘기
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_fire;
	// 총알공장
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	TSubclassOf<class ABullet> bulletFactory;

	// Object Pool
	// 몇발
	UPROPERTY(EditAnywhere, Category=Bullet)
	int32 bulletPoolSize = 10;
	// 탄창
	UPROPERTY()
	TArray<ABullet*> bulletPool;

	// 총알 파편효과(스나이퍼건 쏠때)
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	class UParticleSystem* bulletEffect;
	
	void FireInput(const struct FInputActionValue& value);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_grenadeGun;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_sniperGun;

	bool bUsingGrenade = false;
	
	void ChangeToGrenadeGun(const struct FInputActionValue& value);
	void ChangeToSniperGun(const struct FInputActionValue& value);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_sniperMode;

	// 스나이퍼 UI
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> sniperUIFactory;
	// UI 인스턴스
	UPROPERTY()
	UUserWidget* sniperUI;

	// Crosshair UI
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// UI 인스턴스
	UPROPERTY()
	UUserWidget* crosshairUI;
	
	
	void SniperModeInput(const struct FInputActionValue& value);
};
