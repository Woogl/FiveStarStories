// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EBodyPart.h"
#include "SliceBodyPart.generated.h"

/**
 * ���� ���� �ڵ�
 * ���� ����ũ�ٿ� ���ϴ� �ִϸ��̼� ���� ��ü ��Ʈ �и�
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
