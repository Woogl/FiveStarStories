// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EBodyPart.h"
#include "Dummy02.generated.h"

UCLASS()
class FIVESTARSTORIES_API ADummy02 : public ACharacter
{
	GENERATED_BODY()

public:
	ADummy02();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �÷��̾� �Ĵٺ���
	void LookAtPlayer();
	
	// �´� �ִϸ��̼� ���
	void PerformHitReaction();

	UFUNCTION(BlueprintCallable)
	void OnExecuted();

	// ��ü ����
	UFUNCTION(BlueprintCallable)
	void SliceBodyPart(EBodyPart BodyIndex, FVector Impulse, float RagdollDelay);

	FTimerHandle RagdollTimer;

	// ���׵� Ȱ��ȭ
	UFUNCTION(BlueprintCallable)
	void ActivateRagdoll();

private:
	// �����
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Hit Reactions")
	TArray<UAnimMontage*> HitReactions;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | Executed Reactions")
	TArray<UAnimMontage*> ExecutedReactions;
};
