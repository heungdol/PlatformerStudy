// Fill out your copyright notice in the Description page of Project Settings.


#include "Heung_HangPoint.h"

// Sets default values for this component's properties
UHeung_HangPoint::UHeung_HangPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHeung_HangPoint::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHeung_HangPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

