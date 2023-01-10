// Fill out your copyright notice in the Description page of Project Settings.


#include "SliceCheck.h"
#include "PlayerCharacter.h"

void USliceCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	auto player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player)
	{
		player->SetKatanaCollisionProfileName(TEXT("SliceQuery"));
	}
}

void USliceCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	auto player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player)
	{
		player->SetKatanaCollisionProfileName(TEXT("NoCollision"));
	}
}
