// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerFire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	virtual  void SetInputBinding(class UEnhancedInputComponent* playerInput) override;

	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY()
	class UStaticMeshComponent* sniperComp;
	UPROPERTY()
	class UCameraComponent* camComp;

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
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_grenadeGun;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_sniperGun;

public: // 카메라셰이크
	UPROPERTY(EditDefaultsOnly, Category=CameraMotion)
	TSubclassOf<class UCameraShakeBase> fireCameraShake;

public: // Sound 재생
	UPROPERTY(EditDefaultsOnly, Category=Fire)
	class USoundBase* fireSound;

	bool bUsingGrenade = false;


public:	// sniper
	
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
	
public:
	
	void FireInput(const struct FInputActionValue& value);
	
	void ChangeToGrenadeGun(const struct FInputActionValue& value);
	void ChangeToSniperGun(const struct FInputActionValue& value);
	void SniperModeInput(const struct FInputActionValue& value);

};
