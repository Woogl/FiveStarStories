// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSlicer.h"
#include <Components/BoxComponent.h>
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "SliceableActorBase.h"

AMeshSlicer::AMeshSlicer()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("SliceQuery"));
	// �ڸ� ũ�� ����
	Box->SetBoxExtent(FVector(100.f, 45.f, 0.f));
	RootComponent = Box;

	// ��������Ʈ ���ε�
	Box->OnComponentEndOverlap.AddDynamic(this, &AMeshSlicer::OverlapEnd);

	// ����׿�
	//Box->bHiddenInGame = false;
}

void AMeshSlicer::BeginPlay()
{
	Super::BeginPlay();
	
	// �����ǰ� 0.01�� ������ �ı�
	SetLifeSpan(0.01f);
}

void AMeshSlicer::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// �ܸ鿡 ����� ���͸��� ��������
	auto sliceTarget = Cast<ASliceableActorBase>(OtherActor);
	MatForSlicedSection = sliceTarget->SectionMaterial;

	SliceMesh(OtherComp);
}

void AMeshSlicer::SliceMesh(UPrimitiveComponent* TargetMesh)
{
	// �ڸ� �޽�
	UProceduralMeshComponent* meshToSlice = Cast<UProceduralMeshComponent>(TargetMesh);
	FVector planePosition = Box->GetComponentLocation();
	FVector planeNormal = Box->GetUpVector();

	// �߶��� �޽�
	UProceduralMeshComponent* otherHalfMesh;

	// �޽� �ڸ���
	if (meshToSlice != nullptr)
	{
		if (MatForSlicedSection != nullptr)
		{
			// ������ ���ܸ� ���͸��� ������ �װ� ������
			UKismetProceduralMeshLibrary::SliceProceduralMesh(meshToSlice, planePosition, planeNormal, true, otherHalfMesh, EProcMeshSliceCapOption::CreateNewSectionForCap, MatForSlicedSection);
		}
		else
		{
			// ���� �������� ���� ���͸����� �����
			UKismetProceduralMeshLibrary::SliceProceduralMesh(meshToSlice, planePosition, planeNormal, true, otherHalfMesh, EProcMeshSliceCapOption::UseLastSectionForCap, MatForSlicedSection);
		}
		// �߶��� �޽� �и�
		otherHalfMesh->SetSimulatePhysics(true);
		otherHalfMesh->AddRadialImpulse(planePosition, 1000.f, 500.f, ERadialImpulseFalloff::RIF_Constant, true);
		meshToSlice->AddRadialImpulse(planePosition, 1000.f, 500.f, ERadialImpulseFalloff::RIF_Constant, true);
	}
}

