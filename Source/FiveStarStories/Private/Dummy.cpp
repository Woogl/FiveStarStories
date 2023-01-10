// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

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
	
	// 초기 체력 설정
	CurrentHealth = MaxHealth;
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	// 디버그
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("TakeDamage"));

	// 체력만 깎음
	if (CurrentHealth > 0.f)
	{
		CurrentHealth -= DamageAmount;
	}

	return DamageAmount;
}

void ADummy::OnAttacked(EAttackType AttackType)
{
	// 디버그
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("OnAttacked"));

	if (AttackType == EAttackType::EAT_Standard)
	{
		// 0~4 중에 하나 뽑아서 재생
		HitIndex = UKismetMathLibrary::RandomInteger(5);
		PlayAnimMontage(HitReactions[HitIndex]);
	}
	else if (AttackType == EAttackType::EAT_KnockBack)
	{

	}
	else if (AttackType == EAttackType::EAT_KnockDown)
	{

	}
	else if (AttackType == EAttackType::EAT_KnockUp)
	{

	}
	else
	{
		// EAT_NoResponse
	}
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
	GetCharacterMovement()->SetMovementMode(MOVE_None);
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

