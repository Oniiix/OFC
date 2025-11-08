// Fill out your copyright notice in the Description page of Project Settings.

#include "Duck.h"
#include "Swiper.h"
#include "../Game/BubbleActor.h"
#include "../CaughtComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Game/EnclosureActor.h"

ADuck::ADuck()
{
	PrimaryActorTick.bCanEverTick = true;

	caughtCompo = CreateDefaultSubobject<UCaughtComponent>("CaughtComponent");
	AddOwnedComponent(caughtCompo);
}

void ADuck::BeginPlay()
{
	Super::BeginPlay();

	charaMove = GetComponentByClass<UCharacterMovementComponent>();
	if (!charaMove)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Failed to get CharaMove on " + GetActorNameOrLabel(), true, true, FLinearColor::Red);
		return;
	}
	AIController = Cast<ACustomAIController>(Controller);
	if (!AIController)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Failed to cast Duck's controller in CustomAIController", true, true, FLinearColor::Red);
		return;
	}
	navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!navSystem)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Failed to find navigation system on Duck", true, true, FLinearColor::Red);
		return;
	}

	AIController->OnStopMove().AddDynamic(this, &ADuck::Wait);
	caughtCompo->InitCaught();
	caughtCompo->OnCaught().AddDynamic(this, &ADuck::Bubbled);
	caughtCompo->OnPop().AddDynamic(this, &ADuck::Popped);
	caughtCompo->OnExplode().AddDynamic(this, &ADuck::Death);

	Move();
}

void ADuck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Test swiper distance => fuite
	if (swiper)
	{
		if (FVector::Dist(GetActorLocation(), swiper->GetActorLocation()) <= DetectionDistanceSwiper)
		{
			isChase = true;
			Move();
			GetWorld()->GetTimerManager().ClearTimer(timerWait);
			//UKismetSystemLibrary::PrintString(GetWorld(), "Flee", true, true, FLinearColor::Green);
		}
		else
			isChase = false;
	}
	else
		isChase = false;

	// Debug
	if (drawDebug)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), GetActorLocation(), destination, FLinearColor::Blue, 0.f, 3.f);
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), destination, 25.f, 10, FLinearColor::Blue, 0.f, 3.f);
	}
	UKismetSystemLibrary::PrintString(this, "=============== DUCK ================", true, false, FColor::Green, DeltaTime);
	UKismetSystemLibrary::PrintString(this, isKidnaped ? "is kidnapped" : "Is NOT kidnapped", true, false, isKidnaped ? FColor::Green : FColor::Purple, DeltaTime);
	UKismetSystemLibrary::PrintString(this, isInEnclosure ? "Is in an enclosure" : "Is NOT in an enclosure", true, false, isInEnclosure ? FColor::Green : FColor::Purple, DeltaTime);
	UKismetSystemLibrary::PrintString(this, swiper ? "Has a Swipper" : "Has NOT a Swipper", true, false, swiper ? FColor::Green : FColor::Purple, DeltaTime);
	UKismetSystemLibrary::PrintString(this, isChase ? "IsChase => TRUE" : "IsChase => FALSE", true, false, isChase ? FColor::Green : FColor::Purple, DeltaTime);
	UKismetSystemLibrary::PrintString(this, "=====================================", true, false, FColor::Green, DeltaTime);
}

void ADuck::Wait()
{
	onStop.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(timerWait, this, &ADuck::Move, 2.5f, false);
}

void ADuck::Move()
{
	if (isKidnaped)
		return;

	if (isChase && swiper) // Flee
	{
		charaMove->MaxWalkSpeed = runSpeed;
		const FVector _direction = GetActorLocation() - swiper->GetActorLocation();
		destination = GetActorLocation() + _direction;
		onDetectSwiper.Broadcast();
		AIController->MoveToLocation(destination, 25.f);
	}
	else // Roaming
	{
		charaMove->MaxWalkSpeed = walkSpeed;
		FNavLocation _point;
		for (int i = 0; i < 100; i++)
		{
			const bool _success = navSystem->GetRandomReachablePointInRadius(GetActorLocation(), 500, _point);
			if (_success)
			{
				destination = _point.Location;
				onWalking.Broadcast();
				AIController->MoveToLocation(destination, 25.f);
				break;
			}
		}
	}
}

void ADuck::Bubbled(ABubbleActor* _bubble)
{
	onBubbled.Broadcast();
	isBubbled = true;
	AIController->StopMovement();
	SetActorEnableCollision(false);
	AttachToActor(_bubble, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ADuck::Popped()
{
	isBubbled = false;
	SetActorEnableCollision(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
	onPopBubbled.Broadcast();
	Move();
}

void ADuck::Free()
{
	isKidnaped = false;
	isChase = false;
	swiper = nullptr;
	onFree.Broadcast(this);
	Move();
}

void ADuck::NotFree()
{
	onNotFree.Broadcast(this);
}

void ADuck::Kidnapping()
{
	UKismetSystemLibrary::PrintString(this, "Kidnap Duck", true, true, FColor::Magenta);
	onKidnapped.Broadcast();
	isKidnaped = true;
	AIController->StopMovement();
	SetActorEnableCollision(false);
	if (swiper)
		AttachToComponent(swiper->OffsetDuck(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ADuck::Death()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	onDead.Broadcast(this);
	SetLifeSpan(0.1f);
}

void ADuck::UpdateEnclosure(AEnclosureActor* _enclosure)
{
	enclosure = _enclosure;

	if (enclosure)
		onNotFree.Broadcast(this);
	else
		onFree.Broadcast(this);

	//if (swiper && enclosure)
	//{
	//	swiper->SetTarget(enclosure);
	//	swiper = nullptr;
	//}
}
