// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"

ADummy::ADummy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Ä¸½¶
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.0f);

	// ½ºÄÌ·¹Å» ¸Þ½¬
	ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("SkeletalMesh'/Game/MarketplaceAssets/ARPG_Samurai/Demo/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (meshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(meshAsset.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}
}

void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADummy::PerformHitReaction()
{
	PlayAnimMontage(HitReactions[0]);
}

void ADummy::PerformTakeDownReaction()
{
	PlayAnimMontage(TakeDownReactions[0]);
}

