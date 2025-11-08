// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumUI.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "CustomBlueprintNode.generated.h"

USTRUCT(BlueprintType, Category = "Custom")
struct FListMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	TArray<FString> Items;
};


/**
 * 
 */
UCLASS()
class OFC_API UCustomBlueprintNode : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
			
	UFUNCTION(BlueprintCallable, Category = "Custom|UI") static UUserWidget* GetUserWidgetFromKey(const TMap<UEnumUI, UUserWidget*>& MapMenu, UEnumUI Key);

	UFUNCTION(BlueprintPure, Category = "Custom|Math") static void SelectThreeFloat(const float _a, const float _b, const float _c, float& _firstOut, float& _secondOut);

	UFUNCTION(BlueprintCallable, Category = "Custom|UI") static TArray<FString> GetItemsFromStruct(const FListMap& _struct);

	UFUNCTION(BlueprintCallable, Category = "Custom|UI") static void CreateCategoryStringList(
		TMap<FString, FListMap> _values, FString& _out);
	
};

