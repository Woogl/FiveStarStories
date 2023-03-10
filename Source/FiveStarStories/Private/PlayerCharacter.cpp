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

	// 캡슐
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.0f);

	// 스켈레탈 메쉬
	ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("SkeletalMesh'/Game/MarketplaceAssets/ARPG_Samurai/Demo/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (meshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(meshAsset.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}

	// 칼
	Katana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Katana"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> katanaAsset(TEXT("StaticMesh'/Game/MarketplaceAssets/ARPG_Samurai/Demo/Weapon/Mesh/katana.katana'"));
	if (katanaAsset.Succeeded())
	{
		Katana->SetStaticMesh(katanaAsset.Object);
		Katana->SetupAttachment(GetMesh(), TEXT("katana3"));
		Katana->SetCollisionProfileName(TEXT("Blade"));
	}

	// 칼집
	Scabbard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scabbard"));
	Scabbard->SetupAttachment(GetMesh(), TEXT("scabbard1"));
	Scabbard->SetCollisionProfileName(TEXT("NoCollision"));

	// 스프링암
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;	// 카메라 랙 활성화
	CameraBoom->CameraLagSpeed = 10.f;

	// 카메라
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 회전 설정
	TurnRateGamepad = 50.f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 이동 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 500.f;

	// 공격 판정을 관리하는 컴포넌트
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 기본 무기 설정
	CombatComp->SetMainWeapon(Katana);

	// 기본 무기 최대 콤보 수 설정
	MaxAttackCount = Attacks.Num() - 1;

	// 델리게이트 바인딩
	Katana->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnKatanaBeginOverlap);
	Katana->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnKatanaEndOverlap);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타겟팅한 적 있으면 Yaw 회전
	if (bIsTargeting == true)
	{
		RotateToEnemy();
		// 거리가 멀어지면 타겟 해제
		if (GetDistanceTo(EnemyTarget) > 500.f)
		{
			EnemyTarget = nullptr;
			bIsTargeting = false;
		}
	}
}

// 키 입력
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
	// 공격 가능한 상태인지 체크
	if (CanAttack() == false) return;

	// 오토 타겟팅할 적 탐색
	TryAutoTargeting();

	// 타겟팅한 적을 향해 Yaw 회전
	RotateToEnemy();

	// 공격 분기점
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
	// 방어 가능한 상태인지 체크
	if (CanGuard() == false) return;

	bIsBlocking = true;
	auto animIns = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	animIns->SetIsBlocking(true);
	// 이동속도 감소
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MaxAcceleration = 512.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 32.f;
}

void APlayerCharacter::StopGuard()
{
	bIsBlocking = false;
	auto animIns = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	animIns->SetIsBlocking(false);
	// 이동속도 초기화
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MaxAcceleration = 2048.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 500.f;
}

void APlayerCharacter::Interact()
{

}

void APlayerCharacter::Dash()
{
	// 대시 가능한 상태인지 체크
	if (CanDash() == false) return;

	bIsDashing = true;
	// 이동속도 증가
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void APlayerCharacter::StopDash()
{
	bIsDashing = false;
	// 이동속도 초기화
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

bool APlayerCharacter::CanDoJump()
{
	// TODO: 조건 추가하기

	return true;
}

bool APlayerCharacter::CanAttack()
{
	// TODO: 조건 추가하기

	return true;
}

bool APlayerCharacter::CanGuard()
{
	// TODO: 조건 추가하기

	return true;
}

bool APlayerCharacter::CanInteract()
{
	// TODO: 조건 추가하기

	return true;
}

bool APlayerCharacter::CanDash()
{
	// TODO: 조건 추가하기

	return true;
}

void APlayerCharacter::PerformLightAttack()
{
	// 몽타주 재생
	PlayAnimMontage(Attacks[AttackCount]);

	// 콤보 카운트
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

	// 몽타주 재생
	PlayAnimMontage(JumpAttacks[0]);
}

void APlayerCharacter::PerformHeavyAttack()
{

}

void APlayerCharacter::TryAutoTargeting()
{
	AActor* target = GetNearestEnemy();

	// 타게팅할 적을 찾았을 경우
	if (target != nullptr)
	{
		EnemyTarget = target;
		bIsTargeting = true;
	}
	// 타게팅할 적을 못 찾았을 경우
	else
	{
		EnemyTarget = nullptr;
		bIsTargeting = false;
	}
}

AActor* APlayerCharacter::GetNearestEnemy()
{
	// 트레이스 결과를 저장
	TArray<FHitResult> hits;
	// 트레이스 범위
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * 100.f;
	float radius = 200.f;
	// 찾을 오브젝트 타입 = Pawn
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECC_Pawn);
	objectTypes.Add(pawn);
	// 무시할 오브젝트 타입 = 없음
	TArray< AActor* > actorsToIgnore;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, start, end, radius, objectTypes, false, actorsToIgnore,
		EDrawDebugTrace::None, hits, true);

	// 반환할 액터를 담을 변수
	AActor* nearestEnemy = nullptr;
	float lastDistanceToEnemy = radius + 100.f;
	float distanceToEnemy;

	// 가장 가까운 적 찾기
	for (auto hit : hits)	// 모든 FHitResult에 for문 돌리기
	{
		if (hit.GetActor())
		{
			// 적과의 거리 체크
			distanceToEnemy = GetDistanceTo(hit.GetActor());
			// 가장 가까운 적인지 체크
			if (distanceToEnemy <= lastDistanceToEnemy)
			{
				lastDistanceToEnemy = distanceToEnemy;
				nearestEnemy = hit.GetActor();	// 더 가까우면 nearestEnemy 갱신
				// 디버그
				UKismetSystemLibrary::PrintString(this, TEXT("Nearest Target : ") + nearestEnemy->GetName());
			}
		}
	}

	// 찾은 액터를 반환. 못찾았으면 nullptr 그대로 반환함
	return nearestEnemy;
}

void APlayerCharacter::RotateToEnemy()
{
	// 타겟팅한 적이 있는 경우
	if (EnemyTarget)
	{
		// 적을 향해 Yaw 회전
		FRotator newRotation;
		newRotation.Pitch = GetActorRotation().Pitch;
		newRotation.Roll = GetActorRotation().Roll;
		newRotation.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyTarget->GetActorLocation()).Yaw;
		SetActorRotation(newRotation);
	}
}

void APlayerCharacter::TakeDown()
{
	// TODO: 적이 테이크다운 가능한 상태인지 체크

	PlayAnimMontage(TakeDowns[0]);
}


void APlayerCharacter::SliceMesh()
{
	// TODO: 옮기기
}

void APlayerCharacter::OnKatanaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UKismetSystemLibrary::PrintString(this, TEXT("OnKatanaBeginOverlap"));
}

void APlayerCharacter::OnKatanaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UKismetSystemLibrary::PrintString(this, TEXT("OnKatanaEndOverlap"));

	// 자를 메시
	UProceduralMeshComponent* meshToSlice = Cast<UProceduralMeshComponent>(OtherComp);
	FVector planePosition = Katana->GetComponentLocation();
	FVector planeNormal = Katana->GetUpVector();

	// 잘라진 메시
	UProceduralMeshComponent* otherHalfMesh;

	if (meshToSlice != nullptr)
	{
		// 메시 자르기
		UKismetProceduralMeshLibrary::SliceProceduralMesh(meshToSlice, planePosition, planeNormal, true, otherHalfMesh, EProcMeshSliceCapOption::UseLastSectionForCap, MatForSlicedSection);
		// 잘라진 메시 분리
		otherHalfMesh->SetSimulatePhysics(true);
		otherHalfMesh->AddRadialImpulse(planePosition, 1000.f, 500.f, ERadialImpulseFalloff::RIF_Constant, true);
	}
}

