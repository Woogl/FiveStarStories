// Fill out your copyright notice in the Description page of Project Settings.


#include "SliceBodyPart.h"
#include "Dummy.h"
// Test Code


bool USliceBodyPart::Received_Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequence* AnimSeq) const
{
	auto dummy = Cast<ADummy>(MeshComp->GetOwner());
	dummy->SliceBodyPart(BodyPart, Impulse);
	
	return true;
}
