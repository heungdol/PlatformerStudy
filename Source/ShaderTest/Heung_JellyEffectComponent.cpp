// Fill out your copyright notice in the Description page of Project Settings.


#include "Heung_JellyEffectComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Heung_JellyEffectDataAsset.h"

// Sets default values for this component's properties
UHeung_JellyEffectComponent::UHeung_JellyEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHeung_JellyEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	MyActor = Cast<ACharacter> (GetOwner ());

	if (MyActor != NULL)
	{
		MySkeletalMeshComp = MyActor->GetMesh ();
		if (MySkeletalMeshComp)
		{
			StartScale = MySkeletalMeshComp->GetRelativeScale3D ();
		}

		UActorComponent* ActorComp = MyActor->GetComponentByClass (UCharacterMovementComponent::StaticClass ()); 
		MyCharacterMovementComp = Cast <UCharacterMovementComponent> (ActorComp);
	}
	
	Velocity_Z_CurrentScaleRatio = FVector (1, 1, 1);
	JellyEffectData_CurrentScaleRatio = FVector (1, 1, 1);
}

void UHeung_JellyEffectComponent::JellyEffectByGravity (float DelatTime)
{
	if (MySkeletalMeshComp == NULL)
	{
		return;
	}

	if (MyActor == NULL)
	{
		return;
	}

	Velocity = MyActor->GetVelocity ();

	float Alpha = (FMath::Abs (Velocity.Z) - Velocity_Z_Min) / (Velocity_Z_Max - Velocity_Z_Min);
	Alpha = FMath::Clamp (Alpha, 0., 1.);

	FVector CurrentScale = FMath::Lerp (Scale_Z_Min, Scale_Z_Max, Alpha);
	if (MyCharacterMovementComp->IsFalling ())
	{
		CurrentScale = FMath::Lerp (Scale_Z_Min_InAir, Scale_Z_Max, Alpha);
	}

	Velocity_Z_CurrentScaleRatio = CurrentScale;
}

// Called every frame
void UHeung_JellyEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	JellyEffectByGravity (DeltaTime);

	// MyCharacterMovementComp->

	FVector ResultScale = FMath::VInterpTo (MySkeletalMeshComp->GetRelativeScale3D ()
	, Velocity_Z_CurrentScaleRatio * JellyEffectData_CurrentScaleRatio, DeltaTime, Scale_InterpSpeed);

	ResultScale = ResultScale * StartScale;
	MySkeletalMeshComp->SetRelativeScale3D (ResultScale);
}

void UHeung_JellyEffectComponent::PlayJellyEffectData(UHeung_JellyEffectDataAsset* Data)
{
	if (Data == NULL)
	{
		return;
	}

	StopJellyEffectData ();

	JellyEffectData_Current = Data;
	JellyEffectData_CurrentIndex = -1;

	JellyEffectNextStep ();
}

void UHeung_JellyEffectComponent::StopJellyEffectData()
{
	GetWorld ()->GetTimerManager ().ClearTimer (TimerHandle);

	JellyEffectData_CurrentScaleRatio = FVector (1, 1, 1);
}

void UHeung_JellyEffectComponent::JellyEffectNextStep()
{
	if (JellyEffectData_Current == NULL)
	{
		return;
	}

	if (JellyEffectData_Current->GetJellyEffectValuesCount () < 1)
	{
		return;
	}

	if (JellyEffectData_CurrentIndex >= JellyEffectData_Current->GetJellyEffectValuesCount ())
	{
		return;
	}

	JellyEffectData_CurrentIndex += 1;

	float JellyEffectTime = JellyEffectData_Current->GetJellyEffectTime ();

	float TimePoint0 = 0;
	float TimePoint1 = 1;

	if (JellyEffectData_CurrentIndex == 0)
	{
		TimePoint1 = JellyEffectData_Current->GetJellyEffectValueByIndex (JellyEffectData_CurrentIndex).TimePoint;
		
		JellyEffectData_CurrentScaleRatio = JellyEffectData_Current->GetJellyEffectValueByIndex (JellyEffectData_CurrentIndex).ScaleRatio;
	}
	else if (JellyEffectData_CurrentIndex == JellyEffectData_Current->GetJellyEffectValuesCount ())
	{
		TimePoint0 = JellyEffectData_Current->GetJellyEffectValueByIndex (JellyEffectData_CurrentIndex-1).TimePoint;
		
		JellyEffectData_CurrentScaleRatio = FVector (1, 1, 1);
	}
	else
	{
		TimePoint0 = JellyEffectData_Current->GetJellyEffectValueByIndex (JellyEffectData_CurrentIndex-1).TimePoint;
		TimePoint1 = JellyEffectData_Current->GetJellyEffectValueByIndex (JellyEffectData_CurrentIndex).TimePoint;

		JellyEffectData_CurrentScaleRatio = JellyEffectData_Current->GetJellyEffectValueByIndex (JellyEffectData_CurrentIndex).ScaleRatio;
	}

	JellyEffectTime *= (TimePoint1 - TimePoint0);

	// UE_LOG(LogTemp, Display, TEXT("Gap Time: %f, Point Time0: %f, Point Time0: %f, Scale: %f %f %f"), JellyEffectTime
	// , TimePoint0, TimePoint1 
	// , JellyEffectData_CurrentScaleRatio.X, JellyEffectData_CurrentScaleRatio.Y, JellyEffectData_CurrentScaleRatio.Z);
	
	GetWorld ()->GetTimerManager ().SetTimer (TimerHandle, this, &UHeung_JellyEffectComponent::JellyEffectNextStep, JellyEffectTime);
}
