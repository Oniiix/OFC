// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NavigationSystem.h"
#include "../Game/EnclosureActor.h"
#include "AIManager.generated.h"

#define TABLE_CONTEXT FString("Wave table context")

class ADuck;
class ASwiper;

UCLASS(Blueprintable)
class OFC_API UAIManager : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
	UPROPERTY(BlueprintAssignable)
	FOnGameOver onGameOver;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndWave);
	UPROPERTY(BlueprintAssignable)
	FOnEndWave onEndWave;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishAllWawes);
	UPROPERTY(BlueprintAssignable)
	FOnFinishAllWawes onFinishAllWawes;

	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<AGameModeBase> gameMode = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property")
	TObjectPtr<UDataTable> waves = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property")
	TSubclassOf<ADuck> duckRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property")
	TSubclassOf<ASwiper> swiperRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property")
	TArray<TObjectPtr<ADuck>> listDuck;
	UPROPERTY(EditAnywhere, Category = "Property")
	TArray<TObjectPtr<ADuck>> listFreeDuck;
	UPROPERTY(EditAnywhere, Category = "Property")
	TArray<TObjectPtr<ASwiper>> listSwiper;
	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<AEnclosureActor> enclosure = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Property")
	TArray<AActor*> listSpawnPoint;
	UPROPERTY(EditAnywhere, Category = "Property")
	int spawnDuckRadius = 1500;


	int swiperCount = 0;
	int duckCount = 0;
	int currentWave = 0;
	int waveSwiperMax = 0;
	int swipersDead = 0;

	FTimerHandle timerSpawnSwipers;
	UNavigationSystemV1* navSystem = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	void Init(AGameModeBase* _gameMode);

	UFUNCTION(BlueprintCallable)
	void SetEnclosure(AEnclosureActor* _enclosure);
	UFUNCTION(BlueprintCallable)
	void GetSpawnPoint(TArray<AActor*> _listSpawnPoint);

	UFUNCTION(BlueprintCallable) int GetFreeDuck() const { return listFreeDuck.Num(); }
	UFUNCTION(BlueprintCallable) int GetDuckCount() const { return listDuck.Num(); }
private:
	UFUNCTION()
	void TestWave();
	UFUNCTION(BlueprintCallable)
	void LaunchNextWave();

	UFUNCTION()
	void AddFreeDuck(ADuck* _duck);
	UFUNCTION()
	void RemoveDuck(ADuck* _duck);
	UFUNCTION()
	void RemoveFreeDuck(ADuck* _duck);

	UFUNCTION()
	void SpawnSwipers(const int _count);
	UFUNCTION() void GiveTargetToSwiper(ASwiper* _newSwiper);
	UFUNCTION()
	void SwiperDead(ASwiper* _swiper);
};
