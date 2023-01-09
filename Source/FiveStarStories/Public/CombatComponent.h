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
	UCombatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �ߺ� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<AActor*> AlreadyHitActors;

	// ���� ������ ����� ����. StaticMesh�� "WeaponStart", "WeaponEnd"��� �̸��� ���� �ʿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UStaticMeshComponent* MainWeapon;

	// ���� �β�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float WeaponThickness = 5.0f;

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
	void AttackCheckBegin();

	// ���� ���� ���. �ʿ�� �޽� �����̼� ����
	UFUNCTION(BlueprintCallable)
	void AttackCheck(float InWeaponThickness = 5.0f, FName InStartSocketName = TEXT("WeaponStart"), FName InEndSocketName = TEXT("WeaponEnd"));

	UFUNCTION(BlueprintCallable)
	void AttackCheckEnd();

	// ������ ������ ���ϱ�
	UFUNCTION(BlueprintCallable)
	void DealDamage(AActor* TargetEnemy, float BaseDamage, TSubclassOf<UDamageType> DamageType);
};
