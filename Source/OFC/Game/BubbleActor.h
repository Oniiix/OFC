// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BubbleActor.generated.h"

UCLASS()
class OFC_API ABubbleActor : public AActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartHold);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThrow);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStartHold onStartHold;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnThrow onThrow;


	TObjectPtr<AActor> actorInBubble = nullptr;

public:
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetActorInBubble(AActor* _actor) { actorInBubble = _actor; }
	FORCEINLINE AActor* GetActorInBubble() const { return actorInBubble; }
	FORCEINLINE FOnStartHold& OnStartHold() { return onStartHold; }
	FORCEINLINE FOnThrow& OnThrow() { return onThrow; }

	UFUNCTION(BlueprintImplementableEvent) void PopBubble();
	ABubbleActor();
};
