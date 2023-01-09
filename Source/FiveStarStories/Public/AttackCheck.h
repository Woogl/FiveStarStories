// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class FIVESTARSTORIES_API UAttackCheck : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// 가할 데미지
	UPROPERTY(EditAnywhere, Category = "Custom")
	float BaseDamage = 10.f;
	
	// 메시 슬라이서 스폰할지
	UPROPERTY(EditAnywhere, Category = "Custom")
	bool bShouldSlice = true;

	// 데미지 피격 시 반응 (ex: 경직, 넉백, 다운, 기절 등)
	UPROPERTY(EditAnywhere, Category = "Custom")
	TSubclassOf<UDamageType> DamageType;

	class UCombatComponent* CombatComp;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
