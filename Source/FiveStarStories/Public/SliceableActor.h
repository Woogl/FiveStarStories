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
	ASliceableActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 잘릴 때 생성되는 메시(복사본)의 원본으로만 쓰이는 메시
	class UStaticMeshComponent* OriginalMesh;

	// 실제로 게임에 등장하는 복사본
	class UProceduralMeshComponent* ProceduralMesh;

	// 원본으로 사용할 스태틱메시를 지정해주세요.
	UPROPERTY(EditDefaultsOnly, Category = "Please")
	UStaticMesh* OriginalMeshAsset;
};
