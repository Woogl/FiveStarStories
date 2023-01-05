// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSlicer.h"
#include <Components/BoxComponent.h>
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include <Kismet/KismetSystemLibrary.h>

AMeshSlicer::AMeshSlicer()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("SliceQuery"));
	Box->SetBoxExtent(FVector(10.f, 45.f, 10.f));
	RootComponent = Box;

	// 델리게이트 바인딩
	Box->OnComponentEndOverlap.AddDynamic(this, &AMeshSlicer::OnBoxEndOverlap);

	// 디버그용
	Box->bHiddenInGame = false;
}

void AMeshSlicer::BeginPlay()
{
	Super::BeginPlay();
	
	// 스폰되고 0.1초 지나면 파괴
	SetLifeSpan(0.1f);
}

void AMeshSlicer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeshSlicer::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UKismetSystemLibrary::PrintString(this, TEXT("OnBoxEndOverlap"));

	SliceMesh(OtherComp);
}

void AMeshSlicer::SliceMesh(UPrimitiveComponent* TargetMesh)
{
	// 자를 메시
	UProceduralMeshComponent* meshToSlice = Cast<UProceduralMeshComponent>(TargetMesh);
	FVector planePosition = Box->GetComponentLocation();
	FVector planeNormal = Box->GetUpVector();

	// 잘라진 메시
	UProceduralMeshComponent* otherHalfMesh;

	// 메시 자르기
	if (meshToSlice != nullptr)
	{
		if (MatForSlicedSection != nullptr)
		{
			// 지정된 절단면 머터리얼 있으면 그걸 씌워줌
			UKismetProceduralMeshLibrary::SliceProceduralMesh(meshToSlice, planePosition, planeNormal, true, otherHalfMesh, EProcMeshSliceCapOption::CreateNewSectionForCap, MatForSlicedSection);
		}
		else
		{
			// 지정 안했으면 원래 머터리얼을 덮어씌움
			UKismetProceduralMeshLibrary::SliceProceduralMesh(meshToSlice, planePosition, planeNormal, true, otherHalfMesh, EProcMeshSliceCapOption::UseLastSectionForCap, MatForSlicedSection);
		}
		// 잘라진 메시 분리
		otherHalfMesh->SetSimulatePhysics(true);
		otherHalfMesh->AddRadialImpulse(planePosition, 1000.f, 800.f, ERadialImpulseFalloff::RIF_Constant, true);
		meshToSlice->AddRadialImpulse(planePosition, 1000.f, 800.f, ERadialImpulseFalloff::RIF_Constant, true);
	}
}

