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

	// 잘릴 때 생성되는 메시(복사본)의 원본으로만 쓰이는 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* OriginalMesh;

	// 실제 게임에서 사용할 복사본
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProceduralMeshComponent* ProceduralMesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
