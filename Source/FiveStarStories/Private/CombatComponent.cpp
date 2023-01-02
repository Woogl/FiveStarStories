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
		EDrawDebugTrace::ForDuration, hits, true, FLinearColor::Red, FLinearColor::Green, 0.5f);

	for (auto hit : hits)
	{
		// 이미 맞은 적들이 아닌지 체크
		if (hit.GetActor() && !AlreadyHitActors.Contains(hit.GetActor()))
		{
			// 새로 맞은 적이면 이미 맞은 적들에 추가
			AlreadyHitActors.Add(hit.GetActor());
			
			/* 적이면 Hit Reaction 실행
			if (hit.GetActor()->IsA(ADummy::StaticClass()))
			{
				Cast<ADummy>(hit.GetActor())->PerformHitReaction();
				UE_LOG(LogTemp, Warning, TEXT("Hit"));
			}
			*/

			// TODO: Destructible이면 PerformSliceMesh()
		}
	}
}

void UCombatComponent::ResetWeaponCollision()
{
	// 다시 때릴 수 있도록 초기화
	AlreadyHitActors.Empty();
}

