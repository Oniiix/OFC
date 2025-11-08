// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OFC/Game/BubbleActor.h"
#include "Camera/CameraComponent.h"
#include "CaughtComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFC_API UCaughtComponent : public UActorComponent
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCaught, ABubbleActor*, _bubble);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPop);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplode);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnCaught onCaught;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnPop onPop;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnExplode onExplode;


	UPROPERTY(EditAnywhere, Category="Custom")
	FVector offset;

	UPROPERTY(EditAnywhere, Category="Custom")
	float minSpeed = 1;
	UPROPERTY(EditAnywhere, Category="Custom")
	float maxSpeed = 5;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<ABubbleActor> bubble = nullptr;

	bool isHold = false;
	float currentSpeed = 0;
	FRotator rotateDirection;

	TObjectPtr<UCameraComponent> camera = nullptr;

public:
	FORCEINLINE FOnCaught& OnCaught() { return onCaught; }
	FORCEINLINE FOnPop& OnPop() { return onPop; }
	FORCEINLINE FOnExplode& OnExplode() { return onExplode; }

	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsInBubble() const { return bubble != nullptr; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ABubbleActor* GetBubble() const { return bubble; }

	UCaughtComponent();

	UFUNCTION() void Caught(ABubbleActor* _bubble);
	UFUNCTION() void Pop();
	UFUNCTION() void Explode() {}

	UFUNCTION(BlueprintCallable)
	void InitCaught();
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UFUNCTION() void StartHold();
	UFUNCTION() void Throw();
};
