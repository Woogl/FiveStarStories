// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UCombatComponent::SetDamageInfo(float InBaseDamage, TSubclassOf<UDamageType> InDamageType)
{
	BaseDamage = InBaseDamage;
	DamageType = InDamageType;
}

void UCombatComponent::SetWeapon(UStaticMeshComponent* WeaponMesh, float InWeaponThickness)
{
	MainWeapon = WeaponMesh;
	WeaponThickness = InWeaponThickness;
}

void UCombatComponent::AttackCheckBegin()
{
	// 다시 때릴 수 있도록 초기화
	AlreadyHitActors.Empty();
}

void UCombatComponent::AttackCheckTick()
{
	// 트레이스 결과를 저장
	TArray<FHitResult> hits;
	// 트레이스 범위
	FVector start = MainWeapon->GetSocketLocation(StartPoint);
	FVector end = MainWeapon->GetSocketLocation(EndPoint);
	
	// 찾을 오브젝트 타입 = Pawn, Destructible
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECC_Pawn);
	TEnumAsByte<EObjectTypeQuery> destructible = UEngineTypes::ConvertToObjectType(ECC_Destructible);
	objectTypes.Add(pawn);
	objectTypes.Add(destructible);
	// 무시할 오브젝트 타입 = 없음
	TArray< AActor* > actorsToIgnore;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, start, end, WeaponThickness, objectTypes, false, actorsToIgnore,
		EDrawDebugTrace::None, hits, true);

	for (auto hit : hits)
	{
		auto hitActor = hit.GetActor();
		// 이미 맞은 적들이 아닌지 체크
		if (hitActor && !AlreadyHitActors.Contains(hitActor))
		{
			// 새로 맞은 적이면 이미 맞은 적들에 추가
			AlreadyHitActors.Add(hitActor);

			// 데미지 가하기
			DealDamage(hitActor);
			
			// ECC_Destructible이면 Mesh Slicer 스폰
			if (bEnableSlice == true && hit.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				auto player = Cast<APlayerCharacter>(GetOwner());
				player->SpawnMeshSlicer();
			}

			// 역경직 발생
			if (HitstopTime > 0.f)
			{
				StartHitstop(HitstopTime);
			}
		}
	}
}

void UCombatComponent::AttackCheckEnd()
{
	// 다시 때릴 수 있도록 초기화
	AlreadyHitActors.Empty();
}

void UCombatComponent::DealDamage(AActor* TargetEnemy)
{
	FVector hitFromDirection = GetOwner()->GetActorLocation();
	FHitResult info;
	AController* instigator = GetOwner()->GetInstigatorController();
	AActor* damageCauser = GetOwner();
	
	UGameplayStatics::ApplyPointDamage(TargetEnemy, BaseDamage, hitFromDirection, info, instigator, damageCauser, DamageType);
}

void UCombatComponent::StartHitstop(float Time)
{
	GetOwner()->CustomTimeDilation = 0.01f;
	GetOwner()->GetWorldTimerManager().SetTimer(HitstopTimer, this, &UCombatComponent::EndHitStop, Time, false);
}

void UCombatComponent::EndHitStop()
{
	GetOwner()->CustomTimeDilation = 1.f;
}

