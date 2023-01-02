// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIVESTARSTORIES_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �ߺ� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<AActor*> AlreadyHitActors;

	// ���� ������ ����� ����. StaticMesh�� "WeaponStart", "WeaponEnd"��� �̸��� ���� �ʿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UStaticMeshComponent* MainWeapon;

	// ���� ���� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName StartPoint = TEXT("WeaponStart");

	// ���� ���� ���� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName EndPoint = TEXT("WeaponEnd");

	// ���� ������ ����� ���� ����. �ݵ�� BeginPlay()���� �� �Լ��� ȣ���ؼ� ���⸦ ��������� �۵���
	UFUNCTION(BlueprintCallable)
	void SetMainWeapon(UStaticMeshComponent* WeaponMesh);

	UFUNCTION(BlueprintCallable)
	void DetectWeaponCollision(float WeaponThickness, FName InStartSocketName = TEXT("WeaponStart"), FName InEndSocketName = TEXT("WeaponEnd"));

	UFUNCTION(BlueprintCallable)
	void ResetWeaponCollision();
};
