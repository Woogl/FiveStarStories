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
#include "MeshSlicer.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.0f);

	// 스켈레탈 메쉬
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (meshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(meshAsset.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}

	// 칼
	Katana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Katana"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> katanaAsset(TEXT("StaticMesh'/Game/MarketplaceAssets/ARPG_Samurai/Demo/Weapon/Mesh/katana.katana'"));
	if (katanaAsset.Succeeded())
	{
		Katana->SetStaticMesh(katanaAsset.Object);
		Katana->SetupAttachment(GetMesh(), TEXT("katana3"));
		Katana->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 칼집
	Scabbard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scabbard"));
	Scabbard->SetupAttachment(GetMesh(), TEXT("scabbard1"));
	Scabbard->SetCollisionProfileName(TEXT("NoCollision"));

	// 기본 스프링암
	DefaultCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("DefaultCameraBoom"));
	DefaultCameraBoom->SetupAttachment(RootComponent);
	DefaultCameraBoom->TargetArmLength = 300.0f;
	DefaultCameraBoom->bUsePawnControlRotation = true;
	DefaultCameraBoom->bEnableCameraLag = true;	// 카메라 랙 활성화
	DefaultCameraBoom->CameraLagSpeed = 10.f;

	// 좌측 사이드뷰 스프링암
	LeftCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("LeftCameraBoom"));
	LeftCameraBoom->SetupAttachment(RootComponent);
	LeftCameraBoom->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	LeftCameraBoom->TargetArmLength = 225.f;

	// 우측 사이드뷰 스프링암
	RightCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("RightCameraBoom"));
	RightCameraBoom->SetupAttachment(RootComponent);
	RightCameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	RightCameraBoom->TargetArmLength = 225.f;

	// 카메라
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(DefaultCameraBoom, USpringArmComponent::SocketName);
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

	// 기본 무기, 무기 두께 설정
	CombatComp->SetupWeapon(Katana, 5.0f);

	// 기본 무기 최대 콤보 수 설정
	MaxAttackCount = Attacks.Num() - 1;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타겟팅한 적 있으면 Yaw 회전
	if (bIsTargeting == true)
	{
		RotateToEnemyTarget(DeltaTime, RInterpSpeed);
		// 거리가 멀어지면 타겟 해제
		if (GetDistanceTo(EnemyTarget) > 800.f)
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
	PlayerInputComponent->BindAction("Finisher", IE_Pressed, this, &APlayerCharacter::Finisher);

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

	// TODO : 타겟 바라보게
	if (EnemyTarget)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyTarget->GetActorLocation()));
	}

	// 공격 분기점
	if (GetCharacterMovement()->IsFalling() == true)
	{
		PerformJumpAttack();
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

	// 상태 변수 변경
	bIsBlocking = true;

	// ABP의 스테이트 변경
	auto animIns = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	animIns->SetIsBlocking(true);

	// 이동속도 감소
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MaxAcceleration = 512.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 32.f;
}

void APlayerCharacter::StopGuard()
{
	// 상태 변수 변경
	bIsBlocking = false;

	// ABP의 스테이트 변경
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
	
	// 회피 방향 계산
	FVector inputVector = GetLastMovementInputVector();
	FRotator baseRotation = GetActorRotation();
	float dodgeAngle = CalculateDodgeDirection(inputVector, baseRotation);

	if (bIsTargeting == true)
	{
		// 조준 상태 회피
		if (dodgeAngle >= -45.0f && dodgeAngle <= 45.f)
		{
			PlayAnimMontage(Dodges[0]); // 앞으로
		}
		else if (dodgeAngle > 45.f && dodgeAngle <= 135.f)
		{
			PlayAnimMontage(Dodges[1]); // 오른쪽으로
		}
		else if (dodgeAngle > -135.f && dodgeAngle < -45.f)
		{
			PlayAnimMontage(Dodges[2]); // 왼쪽으로
		}
		else // (dodgeAngle 135.f > && dodgeAngle < -135.f)
		{
			PlayAnimMontage(Dodges[3]); // 뒤로
		}
	}
	else // 비조준 상태 회피
	{
		if (dodgeAngle >= -45.0f && dodgeAngle <= 45.f)
		{
			PlayAnimMontage(Dodges[4]); // 앞으로
		}
		else if (dodgeAngle > 45.f && dodgeAngle <= 135.f)
		{
			PlayAnimMontage(Dodges[5]); // 오른쪽으로
		}
		else if (dodgeAngle > -135.f && dodgeAngle < -45.f)
		{
			PlayAnimMontage(Dodges[6]); // 왼쪽으로
		}
		else // (dodgeAngle 135.f > && dodgeAngle < -135.f)
		{
			PlayAnimMontage(Dodges[7]); // 뒤로
		}
	}

	// 이동속도 증가
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void APlayerCharacter::StopDash()
{
	bIsDashing = false;
	// 이동속도 초기화
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void APlayerCharacter::Finisher()
{
	if (CanAttack() == false) return;

	EnemyTarget = GetNearestEnemy();
	MotionMorph();
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

void APlayerCharacter::PerformJumpAttack()
{
	bIsAttacking = true;

	// 몽타주 재생
	PlayAnimMontage(JumpAttacks[0]);
}

bool APlayerCharacter::TryAutoTargeting()
{
	AActor* target = GetNearestEnemy();

	// 타게팅할 적을 찾았을 경우
	if (target != nullptr)
	{
		EnemyTarget = target;
		bIsTargeting = true;
		return true;
	}
	// 타게팅할 적을 못 찾았을 경우
	else
	{
		EnemyTarget = nullptr;
		bIsTargeting = false;
		return false;
	}

	/* 완성하면 위의 거와 교체할 것
	SearchEnemies();
	ScoreEnemies();
	SetEnemyTarget();
	*/
}

AActor* APlayerCharacter::SearchEnemies()
{
	// 배열 초기화
	SearchedEnemies.Empty();
	DistanceScores.Empty();
	AngleScores.Empty();
	TotalScores.Empty();

	// 트레이스 결과를 저장
	TArray<FHitResult> hits;
	// 트레이스 범위
	FVector start = GetActorLocation();
	FVector end = GetActorLocation();
	float radius = 500.f;
	// 찾을 오브젝트 타입 = Pawn
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECC_Pawn);
	objectTypes.Add(pawn);
	// 무시할 오브젝트 타입
	TArray< AActor* > actorsToIgnore;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, start, end, radius, objectTypes, false, actorsToIgnore,
		EDrawDebugTrace::None, hits, true);

	int i = 0;
	// 모든 FHitResult에 for문 탐색
	for (FHitResult hit : hits)
	{
		AActor* hitActor = hit.GetActor();
		if (hitActor)
		{
			// 찾은 적들에 대해서
			SearchedEnemies.Add(hitActor);
			// 거리 점수 계산 (플레이어 위치 ~ 적 위치)
			DistanceScores.Add(hit.Distance);
			// 각도 점수 계산 (입력 방향 ~ 적을 향하는 방향)
			float temp = FMath::Acos(FVector::DotProduct(GetLastMovementInputVector(), (hitActor->GetActorLocation() - GetActorLocation())));
			AngleScores.Add(temp);
			// 점수 합계
			float temp2 = DistanceScores[i] + AngleScores[i];
			i++;
			TotalScores.Add(temp2);
		}
	}

	// 점수 합계 낮은 적 찾기
	int outIndex = 0;
	float minScore = TotalScores[0];
	for (int j = 0; j < TotalScores.Num(); j++)
	{
		if (minScore > TotalScores[j])
		{
			outIndex = j;
			minScore = TotalScores[j];	// 더 작은 점수가 나오면 교체
		}
	}

	return SearchedEnemies[outIndex];	// 거리 점수 + 각도 점수가 가장 적은 적을 반환
}

void APlayerCharacter::ScoreEnemies()
{

}

void APlayerCharacter::SetEnemyTarget(AActor* Target)
{

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
	for (auto hit : hits)	// 모든 FHitResult에 for문 탐색
	{
		if (hit.GetActor())
		{
			// 적과의 거리 체크
			distanceToEnemy = GetDistanceTo(hit.GetActor());
			// 가장 가까운 적인지 체크. 더 가까우면 nearestEnemy 갱신
			if (distanceToEnemy <= lastDistanceToEnemy)
			{
				lastDistanceToEnemy = distanceToEnemy;
				nearestEnemy = hit.GetActor();
			}
		}
	}

	// 찾은 액터를 반환. 못찾았으면 nullptr 그대로 반환함
	return nearestEnemy;
}

void APlayerCharacter::RotateToEnemyTarget(float DeltaTime, float InterpSpeed)
{	
	// 타겟팅한 적이 있는 경우
	if (EnemyTarget)
	{
		auto temp = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyTarget->GetActorLocation());

		FRotator newRotation;
		newRotation.Pitch = GetActorRotation().Pitch;
		newRotation.Yaw = FMath::RInterpTo(GetActorRotation(), temp, DeltaTime, InterpSpeed).Yaw;
		newRotation.Roll = GetActorRotation().Roll;
		SetActorRotation(newRotation);
	}
}

void APlayerCharacter::FinishEnemy()
{
	PlayAnimMontage(Finishers[0]);
}

float APlayerCharacter::CalculateDodgeDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

void APlayerCharacter::SpawnMeshSlicer()
{
	FActorSpawnParameters spawnParams;
	FTransform spawnTransform = Katana->GetComponentTransform();
	GetWorld()->SpawnActor<AMeshSlicer>(AMeshSlicer::StaticClass(), spawnTransform, spawnParams);
}

void APlayerCharacter::MoveCamera(ECameraPosition CameraPosition)
{
	if (CameraPosition == ECameraPosition::ECP_Default)
	{
		FollowCamera->AttachToComponent(DefaultCameraBoom, FAttachmentTransformRules::KeepWorldTransform);
	}
	else if (CameraPosition == ECameraPosition::ECP_LeftSideView)
	{
		FollowCamera->AttachToComponent(LeftCameraBoom, FAttachmentTransformRules::KeepWorldTransform);
	}
	else if (CameraPosition == ECameraPosition::ECP_RightSideView)
	{
		FollowCamera->AttachToComponent(RightCameraBoom, FAttachmentTransformRules::KeepWorldTransform);
	}

	FLatentActionInfo info;
	info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(FollowCamera, FVector(0.f), FRotator(0.f), false, false, 0.4f, true, EMoveComponentAction::Move, info);
}

