// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SlowMotion.generated.h"

/**
 * ��Ÿ�ֿ��� ���ο��� �ߵ�. BP����(ANS_SlowMotion) ����� ����
 */
UCLASS()
class FIVESTARSTORIES_API USlowMotion : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Custom")
	float TimeDilation = 0.2f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
