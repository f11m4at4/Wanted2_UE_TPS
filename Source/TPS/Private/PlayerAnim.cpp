// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 속도 구하기
	// Pawn -> Player
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<ATPSPlayer>(ownerPawn);
	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();
		FVector right = player->GetActorRightVector();
		speed = FVector::DotProduct(velocity, forward);
		direction = FVector::DotProduct(velocity, right);

		auto cmp = player->GetCharacterMovement();
		isInAir = cmp->IsFalling();
	}
}
