// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPS_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public: // components
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* collisionComp;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* bodyMesh;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* moveComp;

	// 일정시간이 흐르면 없애고 싶다.
	UPROPERTY(EditAnywhere, Category=Settings)
	float destroyTime = 2;
	float currentTime = 0;

	UPROPERTY()
	class ATPSPlayer* player;
	
	// 활성화 세팅 함수
	void SetActive(bool bActive);

	UPROPERTY(EditAnywhere)
	float speed = 10000;

	// 에디터에서 속성을 수정했을 때, 다른 속성들도 수정할 수 있게 하고 싶다.
	virtual  void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};
