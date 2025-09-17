// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"

#include "Bullet.h"
#include "EnemyFSM.h"
#include "EnhancedInputComponent.h"
#include "PlayerAnim.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


// Sets default values for this component's properties
UPlayerFire::UPlayerFire()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputFire(TEXT("'/Game/TPSInput/IA_TPSFire.IA_TPSFire'"));
	if (TempInputFire.Succeeded())
	{
		ia_fire = TempInputFire.Object;
	}

	// 총알효과
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (TempEffect.Succeeded())
	{
		bulletEffect = TempEffect.Object;
	}

	
	// 발사 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase> TempFireSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if (TempFireSound.Succeeded())
	{
		fireSound = TempFireSound.Object;
	}

	
	//  카메라셰이크
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> tempCS(TEXT("'/Game/Blueprints/BP_FireCameraShake.BP_FireCameraShake_C'"));
	if (tempCS.Succeeded())
	{
		fireCameraShake = tempCS.Class;
	}

	
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputGrenadeGun(TEXT("'/Game/TPSInput/IA_TPSGrenadeGun.IA_TPSGrenadeGun'"));
	if (TempInputGrenadeGun.Succeeded())
	{
		ia_grenadeGun = TempInputGrenadeGun.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputSniperGun(TEXT("'/Game/TPSInput/IA_TPSSniperGun.IA_TPSSniperGun'"));
	if (TempInputSniperGun.Succeeded())
	{
		ia_sniperGun = TempInputSniperGun.Object;
	}

	// UI
	static ConstructorHelpers::FClassFinder<UUserWidget> tempCrosshair(TEXT("'/Game/UI/WBP_CrosshairUI.WBP_CrosshairUI_C'"));
	if (tempCrosshair.Succeeded())
	{
		crosshairUIFactory = tempCrosshair.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> tempSniperUI(TEXT("'/Game/UI/WBP_SniperUI.WBP_SniperUI_C'"));
	if (tempSniperUI.Succeeded())
	{
		sniperUIFactory = tempSniperUI.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempInputSniper(TEXT("'/Game/TPSInput/IA_TPSSniper.IA_TPSSniper'"));
	if (TempInputSniper.Succeeded())
	{
		ia_sniperMode = TempInputSniper.Object;
	}
}


// Called when the game starts
void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	gunMeshComp = me->gunMeshComp;
	sniperComp = me->sniperComp;
	camComp = me->camComp;

	// 오브젝트 풀 만들기
	for (int i=0;i<bulletPoolSize;i++)
	{
		// 총알 만들기
		auto bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory);
		// 탄창에 넣기
		bulletPool.Add(bullet);
		// 총알 비활성화
		bullet->SetActive(false);
	}

	// UI 생성
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// crosshair
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	crosshairUI->AddToViewport();
	

	// sniper 활성화
	ChangeToSniperGun(FInputActionValue());

}


void UPlayerFire::SetInputBinding(class UEnhancedInputComponent* playerInput)
{
	Super::SetInputBinding(playerInput);

	// 총쏘기
	playerInput->BindAction(ia_fire, ETriggerEvent::Started, this, &UPlayerFire::FireInput);

	// 총바꾸기
	playerInput->BindAction(ia_grenadeGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToGrenadeGun);
	playerInput->BindAction(ia_sniperGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToSniperGun);

	

	// 스나이퍼모드 전환
	playerInput->BindAction(ia_sniperMode, ETriggerEvent::Started, this, &UPlayerFire::SniperModeInput);
	playerInput->BindAction(ia_sniperMode, ETriggerEvent::Completed, this, &UPlayerFire::SniperModeInput);
}

void UPlayerFire::FireInput(const struct FInputActionValue& value)
{
	// 총발사 애니메이션 재생
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	if (anim)
	{
		anim->PlayAttackAnimation();
	}

	// 카메라 셰이크 재생
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(fireCameraShake);

	// 총발사 사운드 재생
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	
	if (bUsingGrenade)
	{
		// 총알 만들어서 발사시키기
		auto firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));

		// 탄창에서 가져와서 총알 발사하기
		if (bulletPool.Num() > 0)
		{
			auto bullet = bulletPool[0];
			bullet->SetActorTransform(firePosition);
			bullet->SetActive(true);
			// 탄창에서 총알 제거하기
			bulletPool.RemoveAt(0);
		}
	}
	else
	{
		// 스나이퍼건 총발사
		// 총발사해서 부딪힌 지점에 파편을 튀게 해보자
		// 필요정보 : 시작점, 끝점.
		FVector startPos = camComp->GetComponentLocation();
		FVector endPos = startPos + camComp->GetForwardVector() * 10000;

		// 부딪힌정보 저장할 변수
		FHitResult hitInfo;

		FCollisionQueryParams param;
		param.AddIgnoredActor(me);
		// 가상의 선을 쏜다.
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);

		if (bHit)
		{
			// 총알 파편효과 재생
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffect, hitInfo.Location);

			// 부딪힌녀석이 Simulate Physics 가 활성화 되어 있으면
			if (hitInfo.GetComponent()->IsSimulatingPhysics())
			{
				// -> 날려보내자
				hitInfo.GetComponent()->AddImpulseAtLocation( camComp->GetForwardVector() * 1000000, hitInfo.Location);
			}

			// 맞은 녀석이 enemy 면 피격 콜백 호출
			// auto enemy = hitInfo.GetActor()->GetComponentByClass(UEnemyFSM::StaticClass());
			auto enemy = Cast<UEnemyFSM>(hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM")));
			if (enemy)
			{
				enemy->OnDamageProcess(camComp->GetForwardVector());
			}
		}
	}
}


void UPlayerFire::ChangeToGrenadeGun(const struct FInputActionValue& value)
{
	bUsingGrenade = true;
	gunMeshComp->SetVisibility(true);
	sniperComp->SetVisibility(false);
}

void UPlayerFire::ChangeToSniperGun(const struct FInputActionValue& value)
{
	bUsingGrenade = false;
	gunMeshComp->SetVisibility(false);
	sniperComp->SetVisibility(true);
}


void UPlayerFire::SniperModeInput(const struct FInputActionValue& value)
{
	// 유탄총이면 동작하지 않는다.
	if (bUsingGrenade)
	{
		return;
	}

	bool bInput = value.Get<bool>();
	if (bInput)
	{
		sniperUI->AddToViewport();
		camComp->SetFieldOfView(45);
		crosshairUI->RemoveFromParent();
	}
	else
	{
		sniperUI->RemoveFromParent();
		camComp->SetFieldOfView(90);
		crosshairUI->AddToViewport();
	}
}
