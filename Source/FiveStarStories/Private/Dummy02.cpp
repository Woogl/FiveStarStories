// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy02.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/CapsuleComponent.h>

ADummy02::ADummy02()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADummy02::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADummy02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADummy02::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADummy02::LookAtPlayer()
{
	FVector currentLoc = GetActorLocation();
	FVector targetLoc = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	float newYaw = UKismetMathLibrary::FindLookAtRotation(currentLoc, targetLoc).Yaw;
	FRotator newRotator = FRotator(GetActorRotation().Pitch, newYaw, GetActorRotation().Roll);
	SetActorRotation(newRotator);
}

void ADummy02::PerformHitReaction()
{
	PlayAnimMontage(HitReactions[0]);
}

void ADummy02::OnExecuted()
{
	// 플레이어 쳐다보기
	LookAtPlayer();

	// 테이크다운 애니메이션 실행
	PlayAnimMontage(ExecutedReactions[0]);
}

void ADummy02::SliceBodyPart(EBodyPart BodyIndex, FVector Impulse, float RagdollDelay)
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	// 머리 자를 경우
	if (BodyIndex == EBodyPart::EBP_Head)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("head"));
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("neck_01"));
	}
	// 왼팔 자를 경우
	else if (BodyIndex == EBodyPart::EBP_LeftArm)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("upperarm_l"));
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("upperarm_l"));
	}
	// 오른팔 자를 경우
	else if (BodyIndex == EBodyPart::EBP_RightArm)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("upperarm_r"));
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("upperarm_r"));
	}
	// 허리 자를 경우
	else if (BodyIndex == EBodyPart::EBP_Waist)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("spine_01"));
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("spine_01"));
	}
	// 왼다리 자를 경우
	else if (BodyIndex == EBodyPart::EBP_LeftLeg)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("thigh_l"));
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("thigh_l"));
	}
	// 오른다리 자를 경우
	else if (BodyIndex == EBodyPart::EBP_RightLeg)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("thigh_r"));
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("thigh_r"));
	}
	// 엉뚱한 곳 자를 경우
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Invalid Body Part !!!"));
	}

	// 시간 지나면 Ragdoll 활성화
	GetWorldTimerManager().SetTimer(RagdollTimer, this, &ADummy02::ActivateRagdoll, RagdollDelay, false);
}

void ADummy02::ActivateRagdoll()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ActivateRagdoll"));
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

