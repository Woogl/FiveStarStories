// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetSystemLibrary.h>

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::SetMainWeapon(UStaticMeshComponent* WeaponMesh)
{
	MainWeapon = WeaponMesh;
}

void UCombatComponent::DetectWeaponCollision(float WeaponThickness, FName InStartSocketName /*= TEXT("WeaponStart")*/, FName InEndSocketName /*= TEXT("WeaponEnd")*/)
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
		// �̹� ���� ������ �ƴ��� üũ
		if (hit.GetActor() && !AlreadyHitActors.Contains(hit.GetActor()))
		{
			// ���� ���� ���̸� �̹� ���� ���鿡 �߰�
			AlreadyHitActors.Add(hit.GetActor());
			
			/* ���̸� Hit Reaction ����
			if (hit.GetActor()->IsA(ADummy::StaticClass()))
			{
				Cast<ADummy>(hit.GetActor())->PerformHitReaction();
				UE_LOG(LogTemp, Warning, TEXT("Hit"));
			}
			*/

			// TODO: Destructible�̸� PerformSliceMesh()
		}
	}
}

void UCombatComponent::ResetWeaponCollision()
{
	// �ٽ� ���� �� �ֵ��� �ʱ�ȭ
	AlreadyHitActors.Empty();
}

