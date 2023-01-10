// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCheck.h"
#include "CombatComponent.h"

void UAttackCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	CombatComp = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
	if (CombatComp)
	{
		CombatComp->AttackCheckBegin();
		CombatComp->SetDamageInfo(BaseDamage, AttackType);
		CombatComp->HitstopTime = HitStopDuration;
	}
}

void UAttackCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (CombatComp)
	{
		CombatComp->AttackCheckTick();
	}
}

void UAttackCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (CombatComp)
	{
		CombatComp->AttackCheckEnd();
	}
}
