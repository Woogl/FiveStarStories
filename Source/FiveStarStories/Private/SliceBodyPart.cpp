// Fill out your copyright notice in the Description page of Project Settings.


#include "SliceBodyPart.h"
#include "Dummy.h"
// Test Code


void USliceBodyPart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ADummy* dummy = Cast<ADummy>(MeshComp->GetOwner());

	// ��ü �ڸ��� �Լ�
	dummy->SliceBodyPart(BodyIndex, Impulse);
}
