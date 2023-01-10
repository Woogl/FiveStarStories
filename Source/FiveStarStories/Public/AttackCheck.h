// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EAttackType.h"
#include "AttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class FIVESTARSTORIES_API UAttackCheck : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// ���� ������
	UPROPERTY(EditAnywhere, Category = "Custom")
	float BaseDamage = 10.f;
	
	// ������ �ð�
	UPROPERTY(EditAnywhere, Category = "Custom")
	float HitStopDuration;

	// �޽� �����̼� ��������
	UPROPERTY(EditAnywhere, Category = "Custom")
	bool bShouldSlice = true;

	// ���� Ÿ�� (ex: ����, �˹�, �ٿ� ��)
	UPROPERTY(EditAnywhere, Category = "Custom")
	EAttackType AttackType;

	class UCombatComponent* CombatComp;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
