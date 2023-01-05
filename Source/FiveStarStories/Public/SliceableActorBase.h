// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SliceableActorBase.generated.h"

UCLASS()
class FIVESTARSTORIES_API ASliceableActorBase : public AActor
{
	GENERATED_BODY()

public:	
	ASliceableActorBase();

	// �߸� �� �����Ǵ� �޽�(���纻)�� �������θ� ���̴� �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* OriginalMesh;

	// ���� ���ӿ��� ����� ���纻
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProceduralMeshComponent* ProceduralMesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
