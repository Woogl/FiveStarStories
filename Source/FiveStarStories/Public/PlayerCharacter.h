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

protected:
	virtual void BeginPlay() override;

	// 축 입력
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// 액션 입력
	void Attack();
	void Guard();
	void StopGuard();
	void Interact();

	// 오토 타게팅
	bool FindNearestEnemy();
	void RotateToEnemy();

	// 테이크다운
	void TakeDownEnemy();

private:
	// 무브셋
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Attacks")
	TArray<class UAnimMontage*> Attacks;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | TakeDowns")
	TArray<UAnimMontage*> TakeDowns;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Guards")
	TArray<UAnimMontage*> Guards;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Interactions")
	TArray<UAnimMontage*> Interactions;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | HitReactions")
	TArray<UAnimMontage*> HitReactions;

	// 공격 콤보 카운트
	UPROPERTY()
	int AttackCount = 0;

	// 최대 공격 콤보 카운트
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	int MaxAttackCount;

	// 오토 타게팅 상태 여부
	bool bIsTargeting = false;

	// 오토 타게팅 대상
	UPROPERTY()
	AActor* EnemyTarget = nullptr;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
