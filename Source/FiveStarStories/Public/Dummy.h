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
	// �߷��� ��ü ��Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body")
	USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body")
	USkeletalMeshComponent* LeftArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body")
	USkeletalMeshComponent* RightArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body")
	USkeletalMeshComponent* LeftLeg;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body")
	USkeletalMeshComponent* RightLeg;

	// �߷��� �Ŀ� ��ü�� ������ ���� 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Physics Asset")
	UPhysicsAsset* NoHead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Physics Asset")
	UPhysicsAsset* NoLeftArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Physics Asset")
	UPhysicsAsset* NoRightArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Physics Asset")
	UPhysicsAsset* NoLeftLeg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Physics Asset")
	UPhysicsAsset* NoRightLeg;

public:
	ADummy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PerformHitReaction();

	UFUNCTION(BlueprintCallable)
	void OnTakeDown();

	UFUNCTION(BlueprintCallable)
	void PerformTakeDownReaction();

	// ��ü ����
	UFUNCTION(BlueprintCallable)
	void SliceBodyPart(EBodyPart BodyIndex, FVector Impulse);

	// ���׵� Ȱ��ȭ
	UFUNCTION(BlueprintCallable)
	void ActivateRagdoll();

private:
	// �����
	UPROPERTY(EditDefaultsOnly, Category = "Montages | HitReactions")
	TArray<UAnimMontage*> HitReactions;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | TakeDownReactions")
	TArray<UAnimMontage*> TakeDownReactions;

};
