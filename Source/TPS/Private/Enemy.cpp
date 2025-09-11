// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 스켈레탈메시 데이터를 로드하고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth''"));
	// 2. 데이터 로드가 잘 됐다면,
	if (TempMesh.Succeeded())
	{
		//  -> 데이터를 스켈레탈메시 컴포넌트에 할당하고 싶다.
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 3. 위치 설정, 회전 설정 하고 싶다.
		GetMesh()->SetRelativeLocation(FVector(0.000000,0.000000,-87.000000));
		GetMesh()->SetRelativeRotation(FRotator(0.000000,-90,0.000000));
		GetMesh()->SetRelativeScale3D(FVector(0.78f));
	}

	// 이동방향으로 회전 속성 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// fsm
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	// animation blueprint 로드
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

