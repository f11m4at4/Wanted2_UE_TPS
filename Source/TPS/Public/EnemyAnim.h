// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState _state;
	// 공격상태일때 공격 상태 전환할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=FSM)
	bool bAttackPlay = false;

	// attackend 가 됐을 때 처리할 함수
	// 내가 함수를 만들고 블루프린트에 호출도록.
	UFUNCTION(BlueprintCallable, Category=AnimEvent)
	void OnAttackEndEvent();

	// 피격시 호출될 함수
	UFUNCTION(BlueprintImplementableEvent, Category=AnimEvent)
	void PlayDamageAnim(FName sectionName);
	UFUNCTION()
	void AnimNotify_DieEnd();
};
