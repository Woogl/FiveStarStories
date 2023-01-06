// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	EBP_Head = 0 UMETA(DisplayName = "Head"),
	EBP_LeftArm UMETA(DisplayName = "Left Arm"),
	EBP_RightArm UMETA(DisplayName = "Right Arm"),
	EBP_LeftLeg UMETA(DisplayName = "Left Leg"),
	EBP_RightLeg UMETA(DisplayName = "Right Leg"),
};

