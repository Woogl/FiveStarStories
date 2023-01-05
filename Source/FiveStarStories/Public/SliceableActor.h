// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SliceableActor.generated.h"

UCLASS()
class FIVESTARSTORIES_API ASliceableActor : public AActor
{
	GENERATED_BODY()
	
public:	
<<<<<<< Updated upstream:Source/FiveStarStories/Public/SliceableActor.h
	ASliceableActor();
=======
	ASliceableActorBase();

	// �߸� �� �����Ǵ� �޽�(���纻)�� �������θ� ���̴� �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* OriginalMesh;

	// ������ ���ӿ� �����ϴ� ���纻
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProceduralMeshComponent* ProceduralMesh;
>>>>>>> Stashed changes:Source/FiveStarStories/Public/SliceableActorBase.h

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// �߸� �� �����Ǵ� �޽�(���纻)�� �������θ� ���̴� �޽�
	class UStaticMeshComponent* OriginalMesh;

	// ������ ���ӿ� �����ϴ� ���纻
	class UProceduralMeshComponent* ProceduralMesh;

	// �������� ����� ����ƽ�޽ø� �������ּ���.
	UPROPERTY(EditDefaultsOnly, Category = "Please")
	UStaticMesh* OriginalMeshAsset;
};
