// Fill out your copyright notice in the Description page of Project Settings.


#include "EnclosureActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OFC/AI/Swiper.h"

AEnclosureActor::AEnclosureActor()
{

	PrimaryActorTick.bCanEverTick = true;

}

void AEnclosureActor::BeginPlay()
{
	Super::BeginPlay();
	//onIsInitialized.Broadcast(this);
}
