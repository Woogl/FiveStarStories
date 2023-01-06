// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EBodyPart.h"
#include "SliceBodyPart.generated.h"

/**
 * 연구 중인 코드
 * 적이 테이크다운 당하는 애니메이션 도중 신체 파트 분리
 */
UCLASS()
class FIVESTARSTORIES_API USliceBodyPart : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBodyPart BodyIndex = EBodyPart::EBP_Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Impulse;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
