// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomAIController.h"

void ACustomAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	onStopMove.Broadcast();
}
