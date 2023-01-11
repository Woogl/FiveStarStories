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

	// �÷��̾� �Ĵٺ���
	void LookAtPlayer();

	// �¾��� ��
	UFUNCTION(BlueprintCallable)
	void OnAttacked(EAttackType AttackType);

	// ������ ���� ���� ��
	UFUNCTION(BlueprintCallable)
	void OnFinishered();
	
	// ��ü ����
	UFUNCTION(BlueprintCallable)
	void SliceBodyPart(EBodyPart BodyIndex, FVector Impulse, float RagdollDelay);

	// ���׵� Ȱ��ȭ
	UFUNCTION(BlueprintCallable)
	void ActivateRagdoll();

	// ���׵� Ȱ��ȭ Ÿ�̸�
	FTimerHandle RagdollTimer;

private:
	// �����
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
