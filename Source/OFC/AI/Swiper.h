// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Swiper.generated.h"

class ADuck;
class ABubbleActor;
class UCaughtComponent;

UCLASS()
class OFC_API ASwiper : public ACharacter
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDead, ASwiper*, _this);
	FOnDead onDead;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHunting, ASwiper*, _this);
	FOnHunting onHunting;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCatchDuck);
	UPROPERTY(BlueprintAssignable)
	FOnCatchDuck onCatchDuck;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBubbled);
	UPROPERTY(BlueprintAssignable)
	FOnBubbled onBubbled;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPopBubbled);
	UPROPERTY(BlueprintAssignable)
	FOnPopBubbled onPopBubbled;

	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<ACustomAIController> AIController = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<UCharacterMovementComponent> charaMove = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<USceneComponent> offsetDuck = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Property")
	TObjectPtr<UCaughtComponent> caughtCompo = nullptr;

	UPROPERTY(EditAnywhere, Category = "Property")
	int life = 100;
	UPROPERTY(EditAnywhere, Category = "Property")
	int huntSpeed = 600;
	UPROPERTY(EditAnywhere, Category = "Property")
	int escapeSpeed = 800;
	UPROPERTY(EditAnywhere, Category = "Property")
	bool isBubbled = false;
	UPROPERTY(EditAnywhere, Category = "Property")
	TObjectPtr<AActor> target = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property")
	bool drawDebug = false;

	bool hasDuck = false;
	FVector destination;
	FVector start;
	FTimerHandle timerKidnap;

public:
	ASwiper();

	FORCEINLINE FOnDead& OnDead() { return onDead; }
	FORCEINLINE FOnHunting& OnHunting() { return onHunting; }
	FORCEINLINE bool IsBubbled() { return isBubbled; }
	FORCEINLINE TObjectPtr<USceneComponent> OffsetDuck() { return offsetDuck; }

	void SetTarget(AActor* _target);

	UFUNCTION(BlueprintCallable)
	void EnclosureGiveDuck(ADuck* _duck);
	UFUNCTION(BlueprintCallable)
	void Bubbled(ABubbleActor* _bubble);
	UFUNCTION(BlueprintCallable)
	void Popped();
	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION()
	void Move();

private:

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UFUNCTION()
	void Stopped();
};
