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

	// 중복 공격 방지
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<AActor*> AlreadyHitActors;

	// 공격 판정을 계산할 무기. StaticMesh에 "WeaponStart", "WeaponEnd"라는 이름의 소켓 필요
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UStaticMeshComponent* MainWeapon;

	// 무기 두께
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float WeaponThickness = 5.0f;

	// 무기 공격 판정 시작 지점
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName StartPoint = TEXT("WeaponStart");

	// 무기 공격 판정 끝 지점
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName EndPoint = TEXT("WeaponEnd");

	// 공격 판정을 계산할 무기 지정. 반드시 BeginPlay()에서 이 함수를 호출해서 무기를 지정해줘야 작동함
	UFUNCTION(BlueprintCallable)
	void SetMainWeapon(UStaticMeshComponent* WeaponMesh);

	UFUNCTION(BlueprintCallable)
	void AttackCheckBegin();

	// 공격 판정 계산. 필요시 메시 슬라이서 스폰
	UFUNCTION(BlueprintCallable)
	void AttackCheck(float InWeaponThickness = 5.0f, FName InStartSocketName = TEXT("WeaponStart"), FName InEndSocketName = TEXT("WeaponEnd"));

	UFUNCTION(BlueprintCallable)
	void AttackCheckEnd();

	// 적에게 데미지 가하기
	UFUNCTION(BlueprintCallable)
	void DealDamage(AActor* TargetEnemy, float BaseDamage, TSubclassOf<UDamageType> DamageType);
};
