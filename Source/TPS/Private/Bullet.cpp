// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "TPS.h"
#include "TPSPlayer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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
	moveComp->InitialSpeed = speed;
	moveComp->MaxSpeed = speed;
	moveComp->bShouldBounce = true;
	moveComp->Bounciness = 0.2f;

	moveComp->SetUpdatedComponent(collisionComp);

	// InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<ATPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()));
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// currentTime += DeltaTime;
	// if (currentTime > destroyTime)
	// {
	// 	Destroy();
	// }
}


void ABullet::SetActive(bool bActive)
{
	if (bActive)
	{
		// Rendering
		bodyMesh->SetVisibility(true);
		// collision
		collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// 다시 발사 속도 세팅
		moveComp->Velocity = GetActorForwardVector() * moveComp->InitialSpeed;

		// 타이머 이용해서 제거 되도록 처리
		// 무명함수 (Lambda)
		auto DieLambda = [this]()->void
		{
			// Destroy();
			// 탄창에 넣어주자
			if (player)
			{
				SetActive(false);
				player->bulletPool.Add(this);
			}
		};

		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda(DieLambda), destroyTime, false);
	}
	else
	{
		// Rendering
		bodyMesh->SetVisibility(false);
		// collision
		collisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABullet::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// speed 값이 수정이 됐는지 체크
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		moveComp->InitialSpeed = speed;
		moveComp->MaxSpeed = speed;
	}
}
