// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshSlicer.generated.h"

UCLASS()
class FIVESTARSTORIES_API AMeshSlicer : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Box;

public:	
	AMeshSlicer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 절단면 머터리얼 (선택사항. AnimNotify에서도 지정 가능)
	UPROPERTY(EditAnywhere)
	UMaterial* MatForSlicedSection;

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 메시 슬라이스
	UFUNCTION(BlueprintCallable)
	void SliceMesh(UPrimitiveComponent* TargetMesh);

};
