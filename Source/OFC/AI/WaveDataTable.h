// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveDataTable.generated.h"

USTRUCT(BlueprintType)
struct FWaveTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FWaveTableRow() = default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 0, ClampMin = 0))
	int duckToSpawn = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 0, ClampMin = 0))
	int swipersMax = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 0, ClampMin = 0))
	int swiperToSpawn = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 1, ClampMin = 1))
	int swiperFrequency = 0;
};

UCLASS()
class OFC_API UWaveDataTable : public UDataTable
{
	GENERATED_BODY()
	
public:
	UWaveDataTable();
};
