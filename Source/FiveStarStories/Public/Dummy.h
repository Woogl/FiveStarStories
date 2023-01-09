// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EBodyPart.h"
#include "Dummy.generated.h"

UCLASS()
class FIVESTARSTORIES_API ADummy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �÷��̾� �Ĵٺ���
	void LookAtPlayer();

	// �ǰ� �� ȣ��
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// �´� �ִϸ��̼� ���
	void PerformHitReaction();

	// �´� �ִϸ��̼� ���
	void PerformKnockback();

	// ó�� �ߵ��� ��
	UFUNCTION(BlueprintCallable)
	void OnExecuted();

	void Death();
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float hp = 100.f;

	int32 HitIndex;
};
