// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FIVESTARSTORIES_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float TurnRateGamepad;

	// 공격 판정을 관리하는 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UCombatComponent* CombatComp;

	// 테이크다운 시 위치 조정하는 컴포넌트
	//UPROPERTY(VisibleAnywhere, Category = "Combat")
	//class UMotionWarpingComponent* MotionWarpComp;

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

	// 액션 실행 가능 여부 체크
	bool CanDoJump();
	bool CanAttack();
	bool CanGuard();
	bool CanInteract();
	bool CanDash();
	
	// 공격 실행
	void PerformLightAttack();
	void PerformDashAttack();
	void PerformJumpAttack();
	void PerformHeavyAttack();

	// 오토 타게팅 
	void TryAutoTargeting();
	AActor* GetNearestEnemy();
	void RotateToEnemy();

	// 테이크다운
	UFUNCTION(BlueprintCallable)
	void TakeDown();

private:
	// 무브셋
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Attacks")
	TArray<class UAnimMontage*> Attacks;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | TakeDowns")
	TArray<UAnimMontage*> TakeDowns;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Guards")
	TArray<UAnimMontage*> GuardHits;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Guards")
	UAnimMontage* GuardBreak;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Interactions")
	TArray<UAnimMontage*> Interactions;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | HitReactions")
	TArray<UAnimMontage*> HitReactions;

	// 캐릭터 상태
	bool bIsTargeting = false;
	bool bIsAttacking = false;
	bool bIsBlocking = false;
	bool bIsDashing = false;

	// 카메라
	bool bEnableCameraHightlight = false;	// 카메라 연출 발동시키고 싶을 때 true

	// 무기 (0: 카타나, 1: ????, )
	int WeaponIndex = 0;

	// 공격 콤보 카운트
	int AttackCount = 0;
	int MaxAttackCount;

	// 오토 타게팅 대상
	AActor* EnemyTarget = nullptr;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
