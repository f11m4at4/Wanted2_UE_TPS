// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

void UEnemyAnim::OnAttackEndEvent()
{
	bAttackPlay = false;
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	auto enemy = TryGetPawnOwner();
	auto fsm = Cast<UEnemyFSM>(enemy->GetComponentByClass(UEnemyFSM::StaticClass()));
	if (fsm)
	{
		fsm->bDieProcessing = true;
	}
}
