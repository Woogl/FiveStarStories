// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
// Test Code
#include <Kismet/KismetSystemLibrary.h>


ADummy::ADummy()
{
	PrimaryActorTick.bCanEverTick = true;

	// ĸ��
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.0f);

	/* NOTE: ���̷�Ż �޽��� BP���� ���� ����
	// ���̷�Ż �޽�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("SkeletalMesh'/Game/MarketplaceAssets/ARPG_Samurai/Demo/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (meshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(meshAsset.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}
	*/
	
	// �߷��� ��ü ��Ʈ
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetCollisionProfileName(TEXT("NoCollision"));
	Head->SetMasterPoseComponent(GetMesh());
	Head->SetupAttachment(GetMesh());
	Head->bHiddenInGame = true;

	LeftArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftArm"));
	LeftArm->SetCollisionProfileName(TEXT("NoCollision"));
	LeftArm->SetMasterPoseComponent(GetMesh());
	LeftArm->SetupAttachment(GetMesh());
	LeftArm->bHiddenInGame = true;

	RightArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightArm"));
	RightArm->SetCollisionProfileName(TEXT("NoCollision"));
	RightArm->SetMasterPoseComponent(GetMesh());
	RightArm->SetupAttachment(GetMesh());
	RightArm->bHiddenInGame = true;

	LeftLeg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftLeg"));
	LeftLeg->SetCollisionProfileName(TEXT("NoCollision"));
	LeftLeg->SetMasterPoseComponent(GetMesh());
	LeftLeg->SetupAttachment(GetMesh());
	LeftLeg->bHiddenInGame = true;

	RightLeg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightLeg"));
	RightLeg->SetCollisionProfileName(TEXT("NoCollision"));
	RightLeg->SetMasterPoseComponent(GetMesh());
	RightLeg->SetupAttachment(GetMesh());
	RightLeg->bHiddenInGame = true;

	// �߷��� �Ŀ� ��ü�� ������ ���� 
	NoHead = CreateDefaultSubobject<UPhysicsAsset>(TEXT("NoHead"));
	NoLeftArm = CreateDefaultSubobject<UPhysicsAsset>(TEXT("NoLeftArm"));
	NoRightArm = CreateDefaultSubobject<UPhysicsAsset>(TEXT("NoRightArm"));
	NoLeftLeg = CreateDefaultSubobject<UPhysicsAsset>(TEXT("NoLeftLeg"));
	NoRightLeg = CreateDefaultSubobject<UPhysicsAsset>(TEXT("NoRightLeg"));
}

void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADummy::PerformHitReaction()
{
	PlayAnimMontage(HitReactions[0]);
}

void ADummy::OnTakeDown()
{
	// �÷��̾� �Ĵٺ���
	FVector currentLoc = GetActorLocation();
	FVector targetLoc = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	float newYaw = UKismetMathLibrary::FindLookAtRotation(currentLoc, targetLoc).Yaw;
	FRotator newRotator = FRotator(GetActorRotation().Pitch, newYaw, GetActorRotation().Roll);
	SetActorRotation(newRotator);

	// ����ũ�ٿ� �ִϸ��̼� ����
	PerformTakeDownReaction();
}

void ADummy::PerformTakeDownReaction()
{
	PlayAnimMontage(TakeDownReactions[0]);
}

void ADummy::SliceBodyPart(EBodyPart BodyIndex, FVector Impulse)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SliceBodyPart"));

	// ������ ���� ����
	USkeletalMeshComponent* bodyPart = nullptr;
	if (BodyIndex == EBodyPart::EBP_Head)
	{
		bodyPart = Head;
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("head"));
		GetMesh()->HideBoneByName(FName("head"), PBO_None);
		GetMesh()->SetPhysicsAsset(NoHead);	// �ִϸ��̼� ��� �߿� PhysicsAsset �ٲٴϱ� ũ���� �ߴ°ǰ�? NoBody ��δ� C++���� �𸣱���!
	}
	else if (BodyIndex == EBodyPart::EBP_LeftArm)
	{
		bodyPart = LeftArm;
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("upperarm_l"));
		GetMesh()->HideBoneByName(FName("upperarm_l"), PBO_None);
		GetMesh()->SetPhysicsAsset(NoLeftArm);
	}
	else if (BodyIndex == EBodyPart::EBP_RightArm)
	{
		bodyPart = RightArm;
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("upperarm_r"));
		GetMesh()->HideBoneByName(FName("upperarm_r"), PBO_None);
		GetMesh()->SetPhysicsAsset(NoRightArm);
	}
	else if (BodyIndex == EBodyPart::EBP_Waist)
	{
		// TO DO: �㸮 �ڸ���
	}
	else if (BodyIndex == EBodyPart::EBP_LeftLeg)
	{
		bodyPart = LeftLeg;
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("thigh_l"));
		GetMesh()->HideBoneByName(FName("thigh_l"), PBO_None);
		GetMesh()->SetPhysicsAsset(NoLeftLeg);
	}
	else if (BodyIndex == EBodyPart::EBP_RightLeg)
	{
		bodyPart = RightLeg;
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("thigh_r"));
		GetMesh()->HideBoneByName(FName("thigh_r"), PBO_None);
		GetMesh()->SetPhysicsAsset(NoRightLeg);
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Invalid Body Part !!!"));
	}

	// ���ܵ� ���� ����
	bodyPart->bHiddenInGame = false;
	bodyPart->SetMasterPoseComponent(NULL);

	// ���ܵ� ���� ������
	bodyPart->SetCollisionProfileName(TEXT("Ragdoll"));
	bodyPart->SetSimulatePhysics(true);
	bodyPart->AddImpulse(Impulse, NAME_None, true);

	// ĸ�� �ݸ��� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ADummy::ActivateRagdoll()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("thigh_r"));
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

