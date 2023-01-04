// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowMotion.h"
#include <Kismet/GameplayStatics.h>

void USlowMotion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetOwner()->GetWorld(), TimeDilation);
}

void USlowMotion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetOwner()->GetWorld(), 1.f);
}
