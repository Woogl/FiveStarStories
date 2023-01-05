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
	RootComponent = OriginalMesh;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMesh->bUseComplexAsSimpleCollision = false;
	ProceduralMesh->SetSimulatePhysics(true);
	ProceduralMesh->SetCollisionProfileName(TEXT("Destructible"));
	ProceduralMesh->SetupAttachment(RootComponent);

	// OriginalMesh를 복사하여 절차적으로 MeshComponent를 생성
	// * 이 함수가 잘 작동하지 않아 BP 생성자에서 다시 호출해야함
	//UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(OriginalMesh, 0, ProceduralMesh, true);
}

void ASliceableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASliceableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

