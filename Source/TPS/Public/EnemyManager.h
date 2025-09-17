// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPS_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// 적공장에서 적을 랜덤시간에 한번씩 만들어서, 특정위치에 배치하고 싶다.
	// 필요속성 : 시간구간, 특정위치들, 적 공장
	UPROPERTY(EditAnywhere, Category=Spawn)
	float minTime = 1;
	UPROPERTY(EditAnywhere, Category=Spawn)
	float maxTime = 5;

	UPROPERTY(EditAnywhere, Category=Spawn)
	TArray<AActor*> spawnPoints;
	UPROPERTY(EditAnywhere, Category=Spawn)
	TSubclassOf<class AEnemy> enemyFactory;

	FTimerHandle createTimer;

	void CreateEnemy();

	// 스폰할 위치 동적 찾아 할당
	void FindSpawnPoints();
};
