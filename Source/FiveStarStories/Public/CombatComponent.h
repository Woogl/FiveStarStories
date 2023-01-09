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

	// 무기 정보 설정
	UFUNCTION(BlueprintCallable)
	void SetDamageInfo(float InBaseDamage, TSubclassOf<UDamageType> InDamageType);

	// 가할 대미지
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Please")
	float BaseDamage;

	// 대미지 피격 시 반응 (ex : 경직, 다운, 넉백, 공중에 뜸, 기절 등 )
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Please")
	TSubclassOf<UDamageType> DamageType;
	
	// 공격 판정을 계산할 무기 지정. BeginPlay(), 무기 변경 시에 호출해줘야함
	UFUNCTION(BlueprintCallable)
	void SetWeapon(UStaticMeshComponent* WeaponMesh, float InWeaponThickness);

	UFUNCTION(BlueprintCallable)
	void AttackCheckBegin();

	// 공격 판정 계산. 필요시 메시 슬라이서 스폰
	UFUNCTION(BlueprintCallable)
	void AttackCheckTick();

	UFUNCTION(BlueprintCallable)
	void AttackCheckEnd();

	// 적에게 데미지 가하기
	UFUNCTION(BlueprintCallable)
	void DealDamage(AActor* TargetEnemy);

	// 무기 두께
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Please")
	float WeaponThickness = 5.0f;

	// 무기 공격 판정 시작 지점
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Please")
	FName StartPoint = FName("WeaponStart");

	// 무기 공격 판정 끝 지점
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Please")
	FName EndPoint = FName("WeaponEnd");

	// 공격 판정을 계산할 무기. 소켓 2개 이상 필요
	class UStaticMeshComponent* MainWeapon;

	// 중복 공격 방지하기 위한 변수
	TArray<AActor*> AlreadyHitActors;

	// true일 경우에만 자르기 발동
	bool bEnableSlice;
};
