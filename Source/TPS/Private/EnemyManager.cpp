// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	// 맞출 시간
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorldTimerManager().SetTimer(createTimer, this, &AEnemyManager::CreateEnemy, createTime, false);

	FindSpawnPoints();
}

void AEnemyManager::CreateEnemy()
{
	// 위치구하기
	int index = FMath::RandRange(0, spawnPoints.Num()-1);
	auto transform = spawnPoints[index]->GetActorTransform();
	// 스폰해서 위치에 배치하기
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, transform.GetLocation(), transform.Rotator());
		
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorldTimerManager().SetTimer(createTimer, this, &AEnemyManager::CreateEnemy, createTime, false);
}

void AEnemyManager::FindSpawnPoints()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), spawnPoints);
	for (auto spawn: spawnPoints)
	{
		// 이름이 spawnpoint 녀석을 배열에 추가
		if (spawn->GetName().Contains(TEXT("BP_SpawnPoint")))
		{
			spawnPoints.Add(spawn);
		}
	}
}
