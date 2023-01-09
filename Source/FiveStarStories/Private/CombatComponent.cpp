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

void UCombatComponent::SetMainWeapon(UStaticMeshComponent* WeaponMesh)
{
	MainWeapon = WeaponMesh;
}


void UCombatComponent::AttackCheckBegin()
{
	// �ٽ� ���� �� �ֵ��� �ʱ�ȭ
	AlreadyHitActors.Empty();
}

void UCombatComponent::AttackCheck(float InWeaponThickness, FName InStartSocketName, FName InEndSocketName)
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
		EDrawDebugTrace::ForDuration, hits, true, FLinearColor::Red, FLinearColor::Green, 0.5f);

	for (auto hit : hits)
	{
		auto hitActor = hit.GetActor();
		// �̹� ���� ������ �ƴ��� üũ
		if (hitActor && !AlreadyHitActors.Contains(hitActor))
		{
			// ���� ���� ���̸� �̹� ���� ���鿡 �߰�
			AlreadyHitActors.Add(hitActor);

			// ������ ���ϱ�
			//DealDamage(hitActor);
			
			// ECC_Destructible�̸� Mesh Slicer ����
			if (hit.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				auto player = Cast<APlayerCharacter>(GetOwner());
				player->SpawnMeshSlicer();
			}
			
			// TODO : ������ ���ϱ�
		}
	}
}

void UCombatComponent::AttackCheckEnd()
{
	// �ٽ� ���� �� �ֵ��� �ʱ�ȭ
	AlreadyHitActors.Empty();
}

void UCombatComponent::DealDamage(AActor* TargetEnemy, float BaseDamage, TSubclassOf<UDamageType> DamageType)
{
	// �����
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("DealDamage"));

	FVector hitFromDirection = GetOwner()->GetActorLocation();
	FHitResult info;
	AController* instigator = GetOwner()->GetInstigatorController();
	AActor* damageCauser = GetOwner();
	
	UGameplayStatics::ApplyPointDamage(TargetEnemy, BaseDamage, hitFromDirection, info, instigator, damageCauser, DamageType);
}

