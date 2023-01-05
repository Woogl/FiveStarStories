// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "CombatComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "PlayerAnimInstance.h"
#include <Math/UnrealMathUtility.h>
// Test code headers
#include "Dummy.h"	
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// ĸ��
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.0f);

	// ���̷�Ż �޽�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("SkeletalMesh'/Game/MarketplaceAssets/ARPG_Samurai/Demo/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (meshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(meshAsset.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}

	// Į
	Katana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Katana"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> katanaAsset(TEXT("StaticMesh'/Game/MarketplaceAssets/ARPG_Samurai/Demo/Weapon/Mesh/katana.katana'"));
	if (katanaAsset.Succeeded())
	{
		Katana->SetStaticMesh(katanaAsset.Object);
		Katana->SetupAttachment(GetMesh(), TEXT("katana3"));
		Katana->SetCollisionProfileName(TEXT("Blade"));
	}

	// Į��
	Scabbard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scabbard"));
	Scabbard->SetupAttachment(GetMesh(), TEXT("scabbard1"));
	Scabbard->SetCollisionProfileName(TEXT("NoCollision"));

	// ��������
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;	// ī�޶� �� Ȱ��ȭ
	CameraBoom->CameraLagSpeed = 10.f;

	// ī�޶�
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// ȸ�� ����
	TurnRateGamepad = 50.f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// �̵� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 500.f;

	// ���� ������ �����ϴ� ������Ʈ
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �⺻ ���� ����
	CombatComp->SetMainWeapon(Katana);

	// �⺻ ���� �ִ� �޺� �� ����
	MaxAttackCount = Attacks.Num() - 1;

	// ��������Ʈ ���ε�
	Katana->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnKatanaBeginOverlap);
	Katana->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnKatanaEndOverlap);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ÿ������ �� ������ Yaw ȸ��
	if (bIsTargeting == true)
	{
		RotateToEnemy();
		// �Ÿ��� �־����� Ÿ�� ����
		if (GetDistanceTo(EnemyTarget) > 500.f)
		{
			EnemyTarget = nullptr;
			bIsTargeting = false;
		}
	}
}

// Ű �Է�
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &APlayerCharacter::Guard);
	PlayerInputComponent->BindAction("Guard", IE_Released, this, &APlayerCharacter::StopGuard);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &APlayerCharacter::StopDash);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &APlayerCharacter::LookUpAtRate);
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Jump()
{
	if (CanDoJump() == true)
	{
		ACharacter::Jump();
	}
}

void APlayerCharacter::Attack()
{
	// ���� ������ �������� üũ
	if (CanAttack() == false) return;

	// ���� Ÿ������ �� Ž��
	TryAutoTargeting();

	// Ÿ������ ���� ���� Yaw ȸ��
	RotateToEnemy();

	// ���� �б���
	if (GetCharacterMovement()->IsFalling() == true)
	{
		PerformJumpAttack();
	}
	else if (bIsDashing == true)
	{
		PerformDashAttack();
	}
	else if (bIsBlocking == true)
	{
		PerformHeavyAttack();
	}
	else
	{
		PerformLightAttack();
	}
}

void APlayerCharacter::Guard()
{
	// ��� ������ �������� üũ
	if (CanGuard() == false) return;

	bIsBlocking = true;
	auto animIns = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	animIns->SetIsBlocking(true);
	// �̵��ӵ� ����
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MaxAcceleration = 512.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 32.f;
}

void APlayerCharacter::StopGuard()
{
	bIsBlocking = false;
	auto animIns = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	animIns->SetIsBlocking(false);
	// �̵��ӵ� �ʱ�ȭ
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MaxAcceleration = 2048.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 500.f;
}

void APlayerCharacter::Interact()
{

}

void APlayerCharacter::Dash()
{
	// ��� ������ �������� üũ
	if (CanDash() == false) return;

	bIsDashing = true;
	// �̵��ӵ� ����
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void APlayerCharacter::StopDash()
{
	bIsDashing = false;
	// �̵��ӵ� �ʱ�ȭ
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

bool APlayerCharacter::CanDoJump()
{
	// TODO: ���� �߰��ϱ�

	return true;
}

bool APlayerCharacter::CanAttack()
{
	// TODO: ���� �߰��ϱ�

	return true;
}

bool APlayerCharacter::CanGuard()
{
	// TODO: ���� �߰��ϱ�

	return true;
}

bool APlayerCharacter::CanInteract()
{
	// TODO: ���� �߰��ϱ�

	return true;
}

bool APlayerCharacter::CanDash()
{
	// TODO: ���� �߰��ϱ�

	return true;
}

void APlayerCharacter::PerformLightAttack()
{
	// ��Ÿ�� ���
	PlayAnimMontage(Attacks[AttackCount]);

	// �޺� ī��Ʈ
	if (AttackCount < MaxAttackCount)
	{
		AttackCount++;
	}
	else
	{
		AttackCount = 0;
	}
}

void APlayerCharacter::PerformDashAttack()
{

}

void APlayerCharacter::PerformJumpAttack()
{
	bIsAttacking = true;

	// ��Ÿ�� ���
	PlayAnimMontage(JumpAttacks[0]);
}

void APlayerCharacter::PerformHeavyAttack()
{

}

void APlayerCharacter::TryAutoTargeting()
{
	AActor* target = GetNearestEnemy();

	// Ÿ������ ���� ã���� ���
	if (target != nullptr)
	{
		EnemyTarget = target;
		bIsTargeting = true;
	}
	// Ÿ������ ���� �� ã���� ���
	else
	{
		EnemyTarget = nullptr;
		bIsTargeting = false;
	}
}

AActor* APlayerCharacter::GetNearestEnemy()
{
	// Ʈ���̽� ����� ����
	TArray<FHitResult> hits;
	// Ʈ���̽� ����
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * 100.f;
	float radius = 200.f;
	// ã�� ������Ʈ Ÿ�� = Pawn
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECC_Pawn);
	objectTypes.Add(pawn);
	// ������ ������Ʈ Ÿ�� = ����
	TArray< AActor* > actorsToIgnore;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, start, end, radius, objectTypes, false, actorsToIgnore,
		EDrawDebugTrace::None, hits, true);

	// ��ȯ�� ���͸� ���� ����
	AActor* nearestEnemy = nullptr;
	float lastDistanceToEnemy = radius + 100.f;
	float distanceToEnemy;

	// ���� ����� �� ã��
	for (auto hit : hits)	// ��� FHitResult�� for�� ������
	{
		if (hit.GetActor())
		{
			// ������ �Ÿ� üũ
			distanceToEnemy = GetDistanceTo(hit.GetActor());
			// ���� ����� ������ üũ
			if (distanceToEnemy <= lastDistanceToEnemy)
			{
				lastDistanceToEnemy = distanceToEnemy;
				nearestEnemy = hit.GetActor();	// �� ������ nearestEnemy ����
				// �����
				UKismetSystemLibrary::PrintString(this, TEXT("Nearest Target : ") + nearestEnemy->GetName());
			}
		}
	}

	// ã�� ���͸� ��ȯ. ��ã������ nullptr �״�� ��ȯ��
	return nearestEnemy;
}

void APlayerCharacter::RotateToEnemy()
{
	// Ÿ������ ���� �ִ� ���
	if (EnemyTarget)
	{
		// ���� ���� Yaw ȸ��
		FRotator newRotation;
		newRotation.Pitch = GetActorRotation().Pitch;
		newRotation.Roll = GetActorRotation().Roll;
		newRotation.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyTarget->GetActorLocation()).Yaw;
		SetActorRotation(newRotation);
	}
}

void APlayerCharacter::TakeDown()
{
	// TODO: ���� ����ũ�ٿ� ������ �������� üũ

	PlayAnimMontage(TakeDowns[0]);
}


void APlayerCharacter::SliceMesh()
{
	// TODO: �ű��
}

void APlayerCharacter::OnKatanaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UKismetSystemLibrary::PrintString(this, TEXT("OnKatanaBeginOverlap"));
}

void APlayerCharacter::OnKatanaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UKismetSystemLibrary::PrintString(this, TEXT("OnKatanaEndOverlap"));

	// �ڸ� �޽�
	UProceduralMeshComponent* meshToSlice = Cast<UProceduralMeshComponent>(OtherComp);
	FVector planePosition = Katana->GetComponentLocation();
	FVector planeNormal = Katana->GetUpVector();

	// �߶��� �޽�
	UProceduralMeshComponent* otherHalfMesh;

	if (meshToSlice != nullptr)
	{
		// �޽� �ڸ���
		UKismetProceduralMeshLibrary::SliceProceduralMesh(meshToSlice, planePosition, planeNormal, true, otherHalfMesh, EProcMeshSliceCapOption::UseLastSectionForCap, MatForSlicedSection);
		// �߶��� �޽� �и�
		otherHalfMesh->SetSimulatePhysics(true);
		otherHalfMesh->AddRadialImpulse(planePosition, 1000.f, 500.f, ERadialImpulseFalloff::RIF_Constant, true);
	}
}

