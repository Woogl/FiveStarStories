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

	// 캡슐
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.0f);

	/* NOTE: 스켈레탈 메쉬는 BP에서 직접 지정
	// 스켈레탈 메쉬
	ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("SkeletalMesh'/Game/MarketplaceAssets/ARPG_Samurai/Demo/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (meshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(meshAsset.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}
	*/
	
	// 잘려진 신체 파트
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

	// 잘려진 후에 교체용 피직스 에셋 
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
	// 플레이어 쳐다보기
	FVector currentLoc = GetActorLocation();
	FVector targetLoc = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	float newYaw = UKismetMathLibrary::FindLookAtRotation(currentLoc, targetLoc).Yaw;
	FRotator newRotator = FRotator(GetActorRotation().Pitch, newYaw, GetActorRotation().Roll);
	SetActorRotation(newRotator);

	// 테이크다운 애니메이션 실행
	PerformTakeDownReaction();
}

void ADummy::PerformTakeDownReaction()
{
	PlayAnimMontage(TakeDownReactions[0]);
}

void ADummy::SliceBodyPart(EBodyPart BodyIndex, FVector Impulse)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SliceBodyPart"));

	// 절단할 부위 설정
	USkeletalMeshComponent* bodyPart = nullptr;
	if (BodyIndex == EBodyPart::EBP_Head)
	{
		bodyPart = Head;
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("head"));
		GetMesh()->HideBoneByName(FName("head"), PBO_None);
		GetMesh()->SetPhysicsAsset(NoHead);	// 애니메이션 재생 중에 PhysicsAsset 바꾸니까 크래시 뜨는건가? NoBody 경로는 C++에서 모르구나!
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
		// TO DO: 허리 자르기
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

	// 절단된 부위 설정
	bodyPart->bHiddenInGame = false;
	bodyPart->SetMasterPoseComponent(NULL);

	// 절단된 부위 날리기
	bodyPart->SetCollisionProfileName(TEXT("Ragdoll"));
	bodyPart->SetSimulatePhysics(true);
	bodyPart->AddImpulse(Impulse, NAME_None, true);

	// 캡슐 콜리전 변경
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ADummy::ActivateRagdoll()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("thigh_r"));
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

