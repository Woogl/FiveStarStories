// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ECameraPosition.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FIVESTARSTORIES_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* DefaultCameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* LeftCameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* RightCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Katana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Scabbard;

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float TurnRateGamepad;

	// 공격 판정을 관리하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class UCombatComponent* CombatComp;

	// 타게팅 대상
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	AActor* EnemyTarget = nullptr;

	// 카메라 전환
	void MoveCamera(ECameraPosition CameraPosition);

	// 메시 자르는 액터 스폰
	void SpawnMeshSlicer();

protected:
	virtual void BeginPlay() override;

	// 축 입력
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// 액션 입력
	void Jump();
	void Attack();
	void Guard();
	void StopGuard();
	void Interact();
	void Dash();
	void StopDash();
	void Finisher();

	// 액션 실행 가능 여부 체크
	bool CanDoJump();
	bool CanAttack();
	bool CanGuard();
	bool CanInteract();
	bool CanDash();
	
	// 공격 실행
	void PerformLightAttack();
	void PerformJumpAttack();

	// 오토 타게팅 
	void TryAutoTargeting();
	UFUNCTION(BlueprintCallable)
	AActor* GetNearestEnemy();
	void RotateToEnemyTarget(float DeltaTime, float InterpSpeed);

	// 마무리 공격
	UFUNCTION(BlueprintCallable)
	void FinishEnemy();
	// BP에서 이벤트 구현
	UFUNCTION(BlueprintImplementableEvent)
	void MotionMorph();

	// 회피 각도 계산
	float CalculateDodgeDirection(const FVector& Velocity, const FRotator& BaseRotation);

private:
	// 무브셋
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Attacks")
	TArray<class UAnimMontage*> Attacks;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Attacks")
	TArray<class UAnimMontage*> JumpAttacks;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Finishers")
	TArray<UAnimMontage*> Finishers;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Guards")
	TArray<UAnimMontage*> GuardHits;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Guards")
	UAnimMontage* GuardBreak;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Dodges")
	TArray<class UAnimMontage*> Dodges;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Interactions")
	TArray<UAnimMontage*> Interactions;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | HitReactions")
	TArray<UAnimMontage*> HitReactions;

	// 캐릭터 상태
	bool bIsAttacking = false;
	bool bIsBlocking = false;
	bool bIsDashing = false;

	// 카메라
	bool bIsTargeting = false;

	// 무기 데이터 (0=카타나)
	int WeaponIndex = 0;

	// 공격 콤보 카운트
	int AttackCount = 0;
	int MaxAttackCount;

	// Tick에서 적을 바라볼 때 회전 속도 ( 회전 완료까지 1/x 초. 0이면 즉시 회전 완료)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RotationInterpSpeed = 20.f;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return DefaultCameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
