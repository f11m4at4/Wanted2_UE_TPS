// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// root
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("collisionComp"));
	RootComponent = collisionComp;
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bodyMesh"));
	bodyMesh->SetupAttachment(collisionComp);
	bodyMesh->SetRelativeScale3D(FVector(0.62f));
	
	moveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("moveComp"));
	moveComp->InitialSpeed = 10000;
	moveComp->MaxSpeed = 10000;
	moveComp->bShouldBounce = true;
	moveComp->Bounciness = 0.2f;

	moveComp->SetUpdatedComponent(collisionComp);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

