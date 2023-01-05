// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SliceableActorBase.generated.h"

/*
* 잘릴 수 있는 액터를 만들 때 부모로 사용
**/

UCLASS()
class FIVESTARSTORIES_API ASliceableActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASliceableActorBase();

	// 잘릴 때 생성되는 메시(복사본)의 원본으로만 쓰이는 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* OriginalMesh;

	// 실제로 게임에 등장하는 복사본
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProceduralMeshComponent* ProceduralMesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
