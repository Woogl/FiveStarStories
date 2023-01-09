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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom")
	float BaseDamage;

	// ������ �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom")
	float HitstopTime;

	// ����� �ǰ� �� ���� (ex : ����, �ٿ�, �˹�, ���߿� ��, ���� �� )
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom")
	TSubclassOf<UDamageType> DamageType;
	
	// ���� ������ ����� ���� ����. BeginPlay(), ���� ���� �ÿ� ȣ���������
	UFUNCTION(BlueprintCallable)
	void SetupWeapon(UStaticMeshComponent* WeaponMesh, float InWeaponThickness);

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

	// ������
	FTimerHandle HitstopTimer;
	void StartHitstop(float Time);
	void EndHitStop();

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
