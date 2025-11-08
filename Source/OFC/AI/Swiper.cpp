// Fill out your copyright notice in the Description page of Project Settings.

#include "Swiper.h"
#include "NavigationSystem.h"
#include "Swiper.h"
#include "Duck.h"
#include "../Game/BubbleActor.h"
#include "../Game/EnclosureActor.h"
#include "../CaughtComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ASwiper::ASwiper()
{
	PrimaryActorTick.bCanEverTick = true;

	offsetDuck = CreateDefaultSubobject<USceneComponent>("OffsetDuck");
	offsetDuck->SetupAttachment(RootComponent);
	caughtCompo = CreateDefaultSubobject<UCaughtComponent>("CaughtComponent");
	AddOwnedComponent(caughtCompo);
}

void ASwiper::BeginPlay()
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
		UKismetSystemLibrary::PrintString(GetWorld(), "Failed to cast Swiper's controller in CustomAIController", true, true, FLinearColor::Red);
		return;
	}

	AIController->OnStopMove().AddDynamic(this, &ASwiper::Stopped);
	caughtCompo->InitCaught();
	caughtCompo->OnCaught().AddDynamic(this, &ASwiper::Bubbled);
	caughtCompo->OnPop().AddDynamic(this, &ASwiper::Popped);
	caughtCompo->OnExplode().AddDynamic(this, &ASwiper::Death);

	start = GetActorLocation();
	Move();
}

void ASwiper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADuck* _duck = Cast<ADuck>(target);
	if (_duck)
	{
		if (_duck->IsBubbled())
			target = _duck->GetCaughtComponent()->GetBubble();
	}

	// Debug
	if (drawDebug)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), GetActorLocation(), destination, FLinearColor::Red, 0.f, 3.f);
	}

	UKismetSystemLibrary::PrintString(this, "============= SWIPPER ===============", true, false, FColor::Yellow, DeltaTime);
	UKismetSystemLibrary::PrintString(this, target ? "Has a target" : "Has NOT a target", true, false, target ? FColor::Yellow : FColor::Orange, DeltaTime);
	UKismetSystemLibrary::PrintString(this, hasDuck ? "Has a Duck on his Head" : "Has  NOT a Duck on his Head", true, false, hasDuck ? FColor::Yellow : FColor::Orange, DeltaTime);
	UKismetSystemLibrary::PrintString(this, isBubbled ? "Is Bubbled => True" : "is Bubbled => False", true, false, isBubbled ? FColor::Yellow : FColor::Orange, DeltaTime);
	UKismetSystemLibrary::PrintString(this, "=====================================", true, false, FColor::Yellow, DeltaTime);
}

void ASwiper::SetTarget(AActor* _target)
{
	target = _target;
	Move();
}

void ASwiper::EnclosureGiveDuck(ADuck* _duck)
{
	if (_duck)
	{
		target = _duck;
		_duck->SetSwiper(this);
		_duck->Kidnapping();
	}
	GetWorld()->GetTimerManager().SetTimer(timerKidnap, this, &ASwiper::Move, 0.4f, false);
}

void ASwiper::Death()
{
	ADuck* _duck = Cast<ADuck>(target);
	if (_duck)
		_duck->Free();
	onDead.Broadcast(this);
}

void ASwiper::Bubbled(ABubbleActor* _bubble)
{
	if (target)
	{
		ADuck* _duck = Cast<ADuck>(target);
		if (_duck)
		{
			if (hasDuck)
				_duck->Popped(); // wrong name but same action
			_duck->Free();
		}
	}

	onBubbled.Broadcast();
	isBubbled = true;
	hasDuck = false;
	target = nullptr;
	AIController->StopMovement();
	SetActorEnableCollision(false);
	if (_bubble)
		AttachToActor(_bubble, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ASwiper::Popped()
{
	isBubbled = false;
	onPopBubbled.Broadcast();
	SetActorEnableCollision(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
	onHunting.Broadcast(this);
}

void ASwiper::Move()
{
	if (hasDuck) // Escape with duck
	{
		charaMove->MaxWalkSpeed = escapeSpeed;
		destination = start;
		AIController->MoveToLocation(destination, 25.f);
	}
	else if (target && target->IsA<AEnclosureActor>())
	{
		UKismetSystemLibrary::PrintString(this, "TTTTTTTTTESSSSTSTS", true, true, FColor::Magenta);
		charaMove->MaxWalkSpeed = huntSpeed;
		destination = target->GetActorLocation();
		AIController->MoveToActor(target, target->GetComponentsBoundingBox(true, true).GetExtent().X);
	}
	else // Hunt
	{
		if (!target)
			return;
		charaMove->MaxWalkSpeed = huntSpeed;
		destination = target->GetActorLocation();
		AIController->MoveToActor(target, 25.f);
	}
}

void ASwiper::Stopped()
{
	if (isBubbled)
		return;

	ADuck* _duck = Cast<ADuck>(target);
	ABubbleActor* _bubble = Cast<ABubbleActor>(target);
	AEnclosureActor* _enclosure = Cast<AEnclosureActor>(target);
	if (_duck)
	{
		if (hasDuck) // kidnapped a duck and reach the area's outside => hunt again
		{
			UKismetSystemLibrary::PrintString(this, "Stopped", true, true, FColor::Magenta);
			hasDuck = false;
			_duck->Death();
			target = nullptr;
			//GetWorld()->GetTimerManager().SetTimer(timerKidnap, this, &ASwiper::Move, 1.f, false);
			onHunting.Broadcast(this);
		}
		else // Reach a duck => kidnap it
		{
			UKismetSystemLibrary::PrintString(this, "TAke Duck", true, true, FColor::Magenta);

			if (FVector::Dist(GetActorLocation(), _duck->GetActorLocation()) > 100)
				return;
			hasDuck = true;
			_duck->Kidnapping();
			onCatchDuck.Broadcast();
			GetWorld()->GetTimerManager().SetTimer(timerKidnap, this, &ASwiper::Move, 0.5f, false);
		}
	}
	else if (_bubble)
	{
		hasDuck = true;
		target = _bubble->GetActorInBubble();
		_bubble->PopBubble();
		ADuck* _aduck = Cast<ADuck>(target);
		_aduck->Kidnapping();
		onCatchDuck.Broadcast();
		GetWorld()->GetTimerManager().SetTimer(timerKidnap, this, &ASwiper::Move, 0.5f, false);
	} 
	else if (_enclosure)
	{
		UKismetSystemLibrary::PrintString(this, "Enclosure", true, true, FColor::Magenta);
		hasDuck = true;
		_enclosure->AttackEnclosure(this);
		onCatchDuck.Broadcast();
	}
}
