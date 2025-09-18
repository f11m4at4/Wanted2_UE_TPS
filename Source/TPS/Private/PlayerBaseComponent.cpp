// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseComponent.h"

#include "TPS.h"


// Sets default values for this component's properties
UPlayerBaseComponent::UPlayerBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
}


// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// 소유자 액터
	me = Cast<ATPSPlayer>(GetOwner());

	// 델리게이트에 처리 함수 등록
	if (me)
	{
		me->onInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetInputBinding);
	}
}

