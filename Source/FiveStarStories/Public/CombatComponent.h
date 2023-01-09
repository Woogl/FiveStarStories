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

	// ���� ���� ����
	UFUNCTION(BlueprintCallable)
	void SetDamageInfo(float InBaseDamage, TSubclassOf<UDamageType> InDamageType);

	// ���� �����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Please")
	float BaseDamage;

	// ����� �ǰ� �� ���� (ex : ����, �ٿ�, �˹�, ���߿� ��, ���� �� )
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Please")
	TSubclassOf<UDamageType> DamageType;
	
	// ���� ������ ����� ���� ����. BeginPlay(), ���� ���� �ÿ� ȣ���������
	UFUNCTION(BlueprintCallable)
	void SetWeapon(UStaticMeshComponent* WeaponMesh, float InWeaponThickness);

	UFUNCTION(BlueprintCallable)
	void AttackCheckBegin();

	// ���� ���� ���. �ʿ�� �޽� �����̼� ����
	UFUNCTION(BlueprintCallable)
	void AttackCheckTick();

	UFUNCTION(BlueprintCallable)
	void AttackCheckEnd();

	// ������ ������ ���ϱ�
	UFUNCTION(BlueprintCallable)
	void DealDamage(AActor* TargetEnemy);

	// ���� �β�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Please")
	float WeaponThickness = 5.0f;

	// ���� ���� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Please")
	FName StartPoint = FName("WeaponStart");

	// ���� ���� ���� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Please")
	FName EndPoint = FName("WeaponEnd");

	// ���� ������ ����� ����. ���� 2�� �̻� �ʿ�
	class UStaticMeshComponent* MainWeapon;

	// �ߺ� ���� �����ϱ� ���� ����
	TArray<AActor*> AlreadyHitActors;

	// true�� ��쿡�� �ڸ��� �ߵ�
	bool bEnableSlice;
};
