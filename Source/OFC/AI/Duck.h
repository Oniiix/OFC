// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomAIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Duck.generated.h"

class ASwiper;
class ABubbleActor;
class UCaughtComponent;
class AEnclosureActor;

UCLASS()
class OFC_API ADuck : public ACharacter
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDead, ADuck*, _this);
	UPROPERTY(BlueprintAssignable)
	FOnDead onDead;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWalking);
	UPROPERTY(BlueprintAssignable)
	FOnWalking onWalking;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStop);
	UPROPERTY(BlueprintAssignable)
	FOnStop onStop;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetectSwiper);
	UPROPERTY(BlueprintAssignable)
	FOnDetectSwiper onDetectSwiper;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKidnapped);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllwoprivateAccess))
	FOnKidnapped onKidnapped;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBubbled);
	UPROPERTY(BlueprintAssignable)
	FOnBubbled onBubbled;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPopBubbled);
	UPROPERTY(BlueprintAssignable)
	FOnPopBubbled onPopBubbled;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFree, ADuck*, _this);
	FOnFree onFree;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotFree, ADuck*, _this);
	FOnNotFree onNotFree;

	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<ACustomAIController> AIController = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<UCharacterMovementComponent> charaMove = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<UCaughtComponent> caughtCompo = nullptr;

	UPROPERTY(EditAnywhere, Category = "Property")
	int walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category = "Property")
	int runSpeed = 400;
	UPROPERTY(EditAnywhere, Category = "Property")
	int DetectionDistanceSwiper = 800;
	UPROPERTY(EditAnywhere, Category = "Property")
	bool isBubbled = false;
	UPROPERTY(EditAnywhere, Category = "Property")
	bool isKidnaped = false;
	UPROPERTY(EditAnywhere, Category = "Property")
	bool isInEnclosure = false;
	UPROPERTY(EditAnywhere, Category = "Property")
	TObjectPtr<ASwiper> swiper = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property")
	bool drawDebug = false;

	bool isChase = false;
	FVector destination;
	FTimerHandle timerWait;
	UNavigationSystemV1* navSystem = nullptr;

	TObjectPtr<AEnclosureActor> enclosure = nullptr;

public:
	ADuck();

	FORCEINLINE FOnDead& OnDead() { return onDead; }
	FORCEINLINE FOnFree& OnFree() { return onFree; }
	FORCEINLINE FOnNotFree& OnNotFree() { return onNotFree; }
	FORCEINLINE bool IsBubbled() { return isBubbled; }
	FORCEINLINE bool IsKidnaped() { return isKidnaped; }
	FORCEINLINE TObjectPtr<UCaughtComponent> GetCaughtComponent() { return caughtCompo; }

	FORCEINLINE void SetSwiper(ASwiper* _swiper) { swiper = _swiper; }

	UFUNCTION(BlueprintCallable)
	void Bubbled(ABubbleActor* _bubble);
	UFUNCTION(BlueprintCallable)
	void Popped();
	UFUNCTION(BlueprintCallable)
	void Free();
	UFUNCTION(BlueprintCallable)
	void NotFree();
	void Kidnapping();
	UFUNCTION() void Death();
	UFUNCTION(BlueprintCallable)
	void UpdateEnclosure(AEnclosureActor* _enclosure);

private:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UFUNCTION()
	void Move();
	UFUNCTION()
	void Wait();
};
