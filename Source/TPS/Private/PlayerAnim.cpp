// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnim::UPlayerAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempFire(TEXT("/Script/Engine.AnimMontage'/Game/Animations/My_Fire_Rifle_Ironsights_Montage.My_Fire_Rifle_Ironsights_Montage'"));
	if (tempFire.Succeeded())
	{
		attackMontage = tempFire.Object;
	}
}

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

void UPlayerAnim::PlayAttackAnimation()
{
	Montage_Play(attackMontage);
}








