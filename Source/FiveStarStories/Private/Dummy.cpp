// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/CapsuleComponent.h>

ADummy::ADummy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 플레이어의 스프링암에 걸리지 않게
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	// 오토 타게팅할 때 대상으로 걸리지 않게
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 질문 : 적한테는 이 함수가 필요없을거 같은데 지워도 되나?
void ADummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADummy::LookAtPlayer()
{
	FVector currentLoc = GetActorLocation();
	FVector targetLoc = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	float newYaw = UKismetMathLibrary::FindLookAtRotation(currentLoc, targetLoc).Yaw;
	FRotator newRotator = FRotator(GetActorRotation().Pitch, newYaw, GetActorRotation().Roll);
	SetActorRotation(newRotator);
}

float ADummy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (hp > 0.01f)
	{
		hp = hp - DamageAmount;
		//LookAtPlayer();
		PerformHitReaction();
	}
	else
	{
		Death();
	}

	// 디버그
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%f"), hp));

	return hp;
}

void ADummy::PerformHitReaction()
{
	PlayAnimMontage(HitReactions[0]);
}

void ADummy::OnExecuted()
{
	// 플레이어 쳐다보기
	LookAtPlayer();

	// 테이크다운 애니메이션 실행
	PlayAnimMontage(ExecutedReactions[0]);
}

void ADummy::Death()
{
	ActivateRagdoll();
}

void ADummy::SliceBodyPart(EBodyPart BodyIndex, FVector Impulse, float RagdollDelay)
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	// 머리 자를 경우
	if (BodyIndex == EBodyPart::EBP_Head)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("neck_01"));
	}
	// 왼팔 자를 경우
	else if (BodyIndex == EBodyPart::EBP_LeftArm)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("upperarm_l"));
	}
	// 오른팔 자를 경우
	else if (BodyIndex == EBodyPart::EBP_RightArm)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("upperarm_r"));
	}
	// 허리 자를 경우
	else if (BodyIndex == EBodyPart::EBP_Waist)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("spine_01"));
	}
	// 왼다리 자를 경우
	else if (BodyIndex == EBodyPart::EBP_LeftLeg)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("thigh_l"));
	}
	// 오른다리 자를 경우
	else if (BodyIndex == EBodyPart::EBP_RightLeg)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("thigh_r"));
	}
	// 엉뚱한 곳 자를 경우
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Invalid Body Part !!!"));
	}
	// 시간 지나면 Ragdoll 활성화
	if (RagdollDelay <= 0.f)
	{
		ActivateRagdoll();
	}
	else
	{
		GetWorldTimerManager().SetTimer(RagdollTimer, this, &ADummy::ActivateRagdoll, RagdollDelay, false);
	}

	// TODO: 잘라진 양 쪽에 각각 전기 이펙트 발생 루프
}

void ADummy::ActivateRagdoll()
{
	// 디버그
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ActivateRagdoll"));

	// 캡슐 콜리전 변경
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

	// 메시 콜리전 변경
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// 래그돌 활성화
	GetMesh()->SetSimulatePhysics(true);
}

