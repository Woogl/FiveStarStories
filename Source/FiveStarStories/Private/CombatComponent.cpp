// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "PlayerCharacter.h"
#include "Dummy.h"
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


void UCombatComponent::SetDamageInfo(float InBaseDamage, EAttackType InAttackType)
{
	BaseDamage = InBaseDamage;
	AttackType = InAttackType;
}

void UCombatComponent::SetupWeapon(UStaticMeshComponent* WeaponMesh, float InWeaponThickness)
{
	MainWeapon = WeaponMesh;
	WeaponThickness = InWeaponThickness;
}

void UCombatComponent::AttackCheckBegin()
{
	// �ٽ� ���� �� �ֵ��� �ʱ�ȭ
	AlreadyHitActors.Empty();
}

void UCombatComponent::AttackCheckTick()
{
	// Ʈ���̽� ����� ����
	TArray<FHitResult> hits;
	// Ʈ���̽� ����
	FVector start = MainWeapon->GetSocketLocation(StartPoint);
	FVector end = MainWeapon->GetSocketLocation(EndPoint);
	
	// ã�� ������Ʈ Ÿ�� = Pawn, Destructible
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECC_Pawn);
	TEnumAsByte<EObjectTypeQuery> destructible = UEngineTypes::ConvertToObjectType(ECC_Destructible);
	objectTypes.Add(pawn);
	objectTypes.Add(destructible);
	// ������ ������Ʈ Ÿ�� = ����
	TArray< AActor* > actorsToIgnore;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, start, end, WeaponThickness, objectTypes, false, actorsToIgnore,
		EDrawDebugTrace::None, hits, true);

	for (auto hit : hits)
	{
		auto hitActor = hit.GetActor();
		// �̹� ���� ������ �ƴ��� üũ
		if (hitActor && !AlreadyHitActors.Contains(hitActor))
		{
			// ���� ���� ���̸� �̹� ���� ���鿡 �߰�
			AlreadyHitActors.Add(hitActor);

			// ������ ������ ���ϱ�
			if (hit.Component->GetCollisionObjectType() == ECC_Pawn)
			{
				auto target = Cast<ADummy>(hitActor);
				target->OnAttacked(AttackType);
			}
						
			// ECC_Destructible�̸� Mesh Slicer ����
			if (bEnableSlice == true && hit.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				auto player = Cast<APlayerCharacter>(GetOwner());
				if (player)
				{
					player->SpawnMeshSlicer();
				}
			}

			// ������ �߻�
			if (HitstopTime > 0.f)
			{
				StartHitstop(HitstopTime);
			}
		}
	}
}

void UCombatComponent::AttackCheckEnd()
{
	// �ٽ� ���� �� �ֵ��� �ʱ�ȭ
	AlreadyHitActors.Empty();
}

void UCombatComponent::DealDamage(AActor* TargetEnemy)
{
	FVector hitFromDirection = (TargetEnemy->GetActorLocation()) - (GetOwner()->GetActorLocation());
	FHitResult info;
	AController* instigator = GetOwner()->GetInstigatorController();
	AActor* damageCauser = GetOwner();
	
	// ������ ���ϱ�
	UGameplayStatics::ApplyPointDamage(TargetEnemy, BaseDamage, hitFromDirection, info, instigator, damageCauser, nullptr);
}

void UCombatComponent::StartHitstop(float Time)
{
	GetOwner()->CustomTimeDilation = 0.2f;
	GetOwner()->GetWorldTimerManager().SetTimer(HitstopTimer, this, &UCombatComponent::EndHitStop, Time, false);
}

void UCombatComponent::EndHitStop()
{
	GetOwner()->CustomTimeDilation = 1.f;
}

