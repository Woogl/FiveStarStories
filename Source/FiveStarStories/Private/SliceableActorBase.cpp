// Fill out your copyright notice in the Description page of Project Settings.


#include "SliceableActorBase.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

ASliceableActorBase::ASliceableActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	OriginalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OriginalMesh"));
	OriginalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	OriginalMesh->SetHiddenInGame(true);
	OriginalMesh->SetupAttachment(RootComponent);
	RootComponent = OriginalMesh;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMesh->bUseComplexAsSimpleCollision = false;
	ProceduralMesh->SetSimulatePhysics(true);
	ProceduralMesh->SetCollisionProfileName(TEXT("Destructible"));
	ProceduralMesh->SetupAttachment(RootComponent);

	// OriginalMesh�� �����Ͽ� ���������� MeshComponent�� ����
	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(OriginalMesh, 0, ProceduralMesh, true);
}

void ASliceableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASliceableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
