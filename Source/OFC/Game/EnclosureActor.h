// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnclosureActor.generated.h"

class ASwiper;
class ADuck;
class UAIManager;

UCLASS()
class OFC_API AEnclosureActor : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsInitialized, AEnclosureActor*, enclosure);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnIsInitialized onIsInitialized;

public:
	FORCEINLINE FOnIsInitialized& GetOnInitialized() { return onIsInitialized; }

public:
	AEnclosureActor();
	UFUNCTION(BlueprintImplementableEvent) void AttackEnclosure(ASwiper* _swiper);
	UFUNCTION(BlueprintImplementableEvent) void GetSpawnPoints(UAIManager* _aiManager);

protected:
	virtual void BeginPlay() override;
};
