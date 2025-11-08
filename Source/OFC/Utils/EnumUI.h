// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumUI.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "Custom|UI")
enum class UEnumUI : uint8
{
		MainMenu UMETA(DisplayName="Main Menu"),
		Game UMETA(DisplayName="Game"),
		Credits UMETA(DisplayName="Credits"),
		Pause UMETA(DisplayName="Pause"),
		NextWave UMETA(DisplayName="Next Wave"),
		GameOver UMETA(DisplayName="Game Over"),
};

/**
 * 
 */
UENUM(BlueprintType, Category = "Custom|UI")
enum class UEnumUpgradeType : uint8
{
	ShootingRate UMETA(DisplayName="Shooting Rate"),
	SuctionSpeed UMETA(DisplayName="Suction Speed"),
	ShootingPower UMETA(DisplayName="Shooting Power"),
	
};
