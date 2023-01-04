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

	// ���� ������ �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UCombatComponent* CombatComp;

	// ����ũ�ٿ� �� ��ġ �����ϴ� ������Ʈ
	//UPROPERTY(VisibleAnywhere, Category = "Combat")
	//class UMotionWarpingComponent* MotionWarpComp;

protected:
	virtual void BeginPlay() override;

	// �� �Է�
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// �׼� �Է�
	void Jump();
	void Attack();
	void Guard();
	void StopGuard();
	void Interact();
	void Dash();
	void StopDash();

	// �׼� ���� ���� ���� üũ
	bool CanDoJump();
	bool CanAttack();
	bool CanGuard();
	bool CanInteract();
	bool CanDash();
	
	// ���� ����
	void PerformLightAttack();
	void PerformDashAttack();
	void PerformJumpAttack();
	void PerformHeavyAttack();

	// ���� Ÿ���� 
	void TryAutoTargeting();
	AActor* GetNearestEnemy();
	void RotateToEnemy();

	// ����ũ�ٿ�
	UFUNCTION(BlueprintCallable)
	void TakeDown();

private:
	// �����
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

	// ĳ���� ����
	bool bIsTargeting = false;
	bool bIsAttacking = false;
	bool bIsBlocking = false;
	bool bIsDashing = false;

	// ī�޶�
	bool bEnableCameraHightlight = false;	// ī�޶� ���� �ߵ���Ű�� ���� �� true

	// ���� (0: īŸ��, 1: ????, )
	int WeaponIndex = 0;

	// ���� �޺� ī��Ʈ
	int AttackCount = 0;
	int MaxAttackCount;

	// ���� Ÿ���� ���
	AActor* EnemyTarget = nullptr;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
