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

	// �÷��̾��� �������Ͽ� �ɸ��� �ʰ�
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	// ���� Ÿ������ �� ������� �ɸ��� �ʰ�
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
	// �ʱ� ü�� ����
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
	// �����
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("TakeDamage"));

	// ü�¸� ����
	if (CurrentHealth > 0.f)
	{
		CurrentHealth -= DamageAmount;
	}

	return DamageAmount;
}

void ADummy::OnAttacked(EAttackType AttackType)
{
	// �����
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("OnAttacked"));

	if (AttackType == EAttackType::EAT_Standard)
	{
		// 0~4 �߿� �ϳ� �̾Ƽ� ���
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
	// �÷��̾� �Ĵٺ���
	LookAtPlayer();

	// ����ũ�ٿ� �ִϸ��̼� ����
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

	// �Ӹ� �ڸ� ���
	if (BodyIndex == EBodyPart::EBP_Head)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("neck_01"));
	}
	// ���� �ڸ� ���
	else if (BodyIndex == EBodyPart::EBP_LeftArm)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("upperarm_l"));
	}
	// ������ �ڸ� ���
	else if (BodyIndex == EBodyPart::EBP_RightArm)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("upperarm_r"));
	}
	// �㸮 �ڸ� ���
	else if (BodyIndex == EBodyPart::EBP_Waist)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("spine_01"));
	}
	// �޴ٸ� �ڸ� ���
	else if (BodyIndex == EBodyPart::EBP_LeftLeg)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("thigh_l"));
	}
	// �����ٸ� �ڸ� ���
	else if (BodyIndex == EBodyPart::EBP_RightLeg)
	{
		GetMesh()->BreakConstraint(Impulse, GetActorLocation(), FName("thigh_r"));
	}
	// ������ �� �ڸ� ���
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Invalid Body Part !!!"));
	}
	// �ð� ������ Ragdoll Ȱ��ȭ
	if (RagdollDelay <= 0.f)
	{
		ActivateRagdoll();
	}
	else
	{
		GetWorldTimerManager().SetTimer(RagdollTimer, this, &ADummy::ActivateRagdoll, RagdollDelay, false);
	}

	// TODO: �߶��� �� �ʿ� ���� ���� ����Ʈ �߻� ����
}

void ADummy::ActivateRagdoll()
{
	// �����
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ActivateRagdoll"));

	// ĸ�� �ݸ��� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

	// �޽� �ݸ��� ����
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// ���׵� Ȱ��ȭ
	GetMesh()->SetSimulatePhysics(true);
}

