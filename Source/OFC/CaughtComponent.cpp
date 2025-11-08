// Fill out your copyright notice in the Description page of Project Settings.


#include "CaughtComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UCaughtComponent::UCaughtComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCaughtComponent::Caught(ABubbleActor* _bubble)
{
	rotateDirection = FRotator(FMath::RandRange(0, 1), FMath::RandRange(0, 1), FMath::RandRange(0, 1));
	currentSpeed = FMath::RandRange(minSpeed, maxSpeed);
	bubble = _bubble;
	camera = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UCameraComponent>();
	bubble->OnStartHold().AddDynamic(this, &UCaughtComponent::StartHold);
	bubble->OnThrow().AddDynamic(this, &UCaughtComponent::Throw);
}

void UCaughtComponent::Pop()
{
	bubble = nullptr;
	GetOwner()->SetActorRotation(FRotator(0));
}

void UCaughtComponent::InitCaught()
{
	onCaught.AddDynamic(this, &UCaughtComponent::Caught);
	onPop.AddDynamic(this, &UCaughtComponent::Pop);
	onExplode.AddDynamic(this, &UCaughtComponent::Explode);
}

void UCaughtComponent::StartHold()
{
	isHold = true;
}

void UCaughtComponent::Throw()
{
	isHold = false;
}

void UCaughtComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bubble)
	{
		if (isHold)
			GetOwner()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), camera->GetComponentLocation()));
		else
			GetOwner()->AddActorLocalRotation(rotateDirection * currentSpeed);
	}
}
