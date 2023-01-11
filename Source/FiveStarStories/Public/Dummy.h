// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EBodyPart.h"
#include "EAttackType.h"
#include "Dummy.generated.h"

UCLASS()
class FIVESTARSTORIES_API ADummy : public ACharacter
{
	GENERATED_BODY()

public:
	ADummy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 플레이어 쳐다보기
	void LookAtPlayer();

	// 맞았을 때
	UFUNCTION(BlueprintCallable)
	void OnAttacked(EAttackType AttackType);

	// 마무리 공격 당할 때
	UFUNCTION(BlueprintCallable)
	void OnFinishered();
	
	// 신체 절단
	UFUNCTION(BlueprintCallable)
	void SliceBodyPart(EBodyPart BodyIndex, FVector Impulse, float RagdollDelay);

	// 래그돌 활성화
	UFUNCTION(BlueprintCallable)
	void ActivateRagdoll();

	// 래그돌 활성화 타이머
	FTimerHandle RagdollTimer;

private:
	// 무브셋
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Hit Reactions")
	TArray<UAnimMontage*> HitReactions;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Hit Reactions")
	TArray<UAnimMontage*> Knockbacks;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Executed Reactions")
	TArray<UAnimMontage*> ExecutedReactions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	int32 HitIndex;
};
