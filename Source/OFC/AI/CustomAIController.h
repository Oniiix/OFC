// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CustomAIController.generated.h"

UCLASS()
class OFC_API ACustomAIController : public AAIController
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopMove);
	FOnStopMove onStopMove;

public:
	FORCEINLINE FOnStopMove& OnStopMove() { return onStopMove; }

private:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
