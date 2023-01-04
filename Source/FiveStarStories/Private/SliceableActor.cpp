// Fill out your copyright notice in the Description page of Project Settings.


#include "SliceableActor.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
ASliceableActor::ASliceableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	OriginalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OriginalMesh"));
	OriginalMesh->SetStaticMesh(OriginalMeshAsset);
	OriginalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	OriginalMesh->SetHiddenInGame(true);

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMesh->bUseComplexAsSimpleCollision = false;
	ProceduralMesh->SetSimulatePhysics(true);
	ProceduralMesh->SetCollisionProfileName(TEXT("Destructible"));
	
	// OriginalMesh를 복사하여 절차적으로 MeshComponent를 생성
	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(OriginalMesh, 0, ProceduralMesh, true);
}

void ASliceableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASliceableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
