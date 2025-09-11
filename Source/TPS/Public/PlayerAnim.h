// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnim();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	float direction = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	bool isInAir = false;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	class UAnimMontage* attackMontage;
	// 공격 애니메이션 재생 함수
	void PlayAttackAnimation();
};








