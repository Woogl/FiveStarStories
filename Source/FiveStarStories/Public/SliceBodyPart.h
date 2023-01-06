// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EBodyPartIndex.h"
#include "SliceBodyPart.generated.h"

/**
 * ���� ����ũ�ٿ� ���ϴ� �ִϸ��̼� ���� ��ü ��Ʈ �и�
 */
UCLASS()
class FIVESTARSTORIES_API USliceBodyPart : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* BodyPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Impulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBodyPart BodyIndex = EBodyPart::EBP_Head;

	virtual bool Received_Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequence* AnimSeq) const;
};
