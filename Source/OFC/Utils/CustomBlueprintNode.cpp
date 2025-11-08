// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBlueprintNode.h"

#include <ranges>


UUserWidget* UCustomBlueprintNode::GetUserWidgetFromKey(const TMap<UEnumUI, UUserWidget*>& MapMenu, const UEnumUI Key)
{
	return MapMenu[Key];
}

void UCustomBlueprintNode::SelectThreeFloat(const float _a, const float _b, const float _c, float& _firstOut, float& _secondOut)
{
	if (_a > _b)
	{
		_firstOut = _a;
		_secondOut = _b > _c ? _b : _c;
	}
	else
	{
		_firstOut = _b;
		_secondOut = _a > _c ? _a : _c;
	}
}

TArray<FString> UCustomBlueprintNode::GetItemsFromStruct(const FListMap& _struct)
{
	return _struct.Items;
}

void UCustomBlueprintNode::CreateCategoryStringList(TMap<FString, FListMap> _values, FString& _out)
{
	
	for (const TTuple<FString, FListMap>& Value : _values)
	{
		_out.Append(Value.Key);
		_out.Append("\n");

		for (const FString& Item : Value.Value.Items)
		{
			GLog->Log(Item);
			_out.Append(Item);
			_out.Append("\n");
		}

		_out.Append("\n\n");
	}
}