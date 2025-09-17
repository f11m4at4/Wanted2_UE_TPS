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



public: // 카메라셰이크
	UPROPERTY(EditDefaultsOnly, Category=CameraMotion)
	TSubclassOf<class UCameraShakeBase> fireCameraShake;

public: // Sound 재생
	UPROPERTY(EditDefaultsOnly, Category=Fire)
	class USoundBase* fireSound;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerBaseComponent* playerMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerBaseComponent* playerFire;
};











