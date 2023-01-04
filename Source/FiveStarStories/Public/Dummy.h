// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/CapsuleComponent.h>
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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PerformHitReaction();

	UFUNCTION(BlueprintCallable)
	void PerformTakeDownReaction();

private:
	// ¹«ºê¼Â
	UPROPERTY(EditDefaultsOnly, Category = "Montages | HitReactions")
	TArray<UAnimMontage*> HitReactions;

	UPROPERTY(EditDefaultsOnly, Category = "Montages | TakeDownReactions")
	TArray<UAnimMontage*> TakeDownReactions;
};
