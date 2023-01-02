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
		Katana->SetCollisionProfileName(TEXT("NoCollision"));
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
	CameraBoom->CameraLagSpeed = 8.0f;

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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

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
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ÿ������ �� ������ Yaw ȸ��
	if (bIsTargeting)
	{
		RotateToEnemy();
		// �Ÿ��� �־����� Ÿ�� ����
		if (GetDistanceTo(EnemyTarget) > 600.f)
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &APlayerCharacter::Guard);
	PlayerInputComponent->BindAction("Guard", IE_Released, this, &APlayerCharacter::StopGuard);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);

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

void APlayerCharacter::Attack()
{
	// TODO: ���¿� ���� ������ �� �ִ��� �Ǵ�

	// ���� Ÿ������ �� Ž��
	FindNearestEnemy();

	// Ÿ������ ���� ���� Yaw ȸ��
	RotateToEnemy();

	// ���� ��Ÿ�� ����
	UKismetSystemLibrary::PrintString(this, TEXT("Attack Count : ") + FString::FromInt(AttackCount));
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

void APlayerCharacter::Guard()
{

}

void APlayerCharacter::StopGuard()
{

}

void APlayerCharacter::Interact()
{

}

bool APlayerCharacter::FindNearestEnemy()
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
		EDrawDebugTrace::ForDuration, hits, true);

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
				UKismetSystemLibrary::PrintString(this, TEXT("Auto Target : ") + nearestEnemy->GetName());
			}
		}
	}

	// nearestEnemy�� ã���� ��쿡�� ���͸� ��ȯ
	if (nearestEnemy)
	{
		EnemyTarget = nearestEnemy;
		bIsTargeting = true;
		return true;
	}
	else
	{
		bIsTargeting = false;
		return false;
	}
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

void APlayerCharacter::TakeDownEnemy()
{
	FindNearestEnemy();

	// TODO: ���� ����ũ�ٿ� ������ �������� Ȯ���ϰ� ����ũ�ٿ�
}