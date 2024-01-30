// Fill out your copyright notice in the Description page of Project Settings.


#include "Heung_LimitWorldLocationZ.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UHeung_LimitWorldLocationZ::UHeung_LimitWorldLocationZ()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHeung_LimitWorldLocationZ::BeginPlay()
{
	Super::BeginPlay();

	// ...

	MyActor = GetOwner ();
	MyCharacter = Cast <ACharacter> (MyActor);

	if (MyCharacter != NULL)
	{
		CharacterSkeletalMeshComp = MyCharacter->GetMesh ();
	}

	if (CharacterSkeletalMeshComp != NULL)
	{
		LimitLocationZ_Min = CharacterSkeletalMeshComp->GetComponentLocation ().Z;
		LimitLocationZ_Max = LimitLocationZ_Min + LimitRange;

		PivotLocation_Prev = CharacterSkeletalMeshComp->GetComponentLocation ();
		PivotLocation_Next = CharacterSkeletalMeshComp->GetComponentLocation ();

		PivotLocation_Prev.Z = LimitLocationZ_Min; 
		PivotLocation_Next.Z = LimitLocationZ_Min; 
	}
	
	if (MyActor != NULL )
	{
		UActorComponent* ActorComp = MyActor->GetComponentByClass (UCharacterMovementComponent::StaticClass ());
		CharacterMovementComp = Cast <UCharacterMovementComponent> (ActorComp);
	}
}


// Called every frame
void UHeung_LimitWorldLocationZ::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	bool IsRawMove = false;

	if (CharacterSkeletalMeshComp != NULL && CharacterMovementComp != NULL)
	{
		if (CharacterMovementComp->IsFalling() == false)
		{
			IsRawMove = false;

			LimitLocationZ_Min = CharacterSkeletalMeshComp->GetComponentLocation ().Z;
			LimitLocationZ_Max = LimitLocationZ_Min + LimitRange;
			
			// CurrentPivotLocation = LimitLocationZ_Min;
		}
		else
		{
			IsRawMove = true;
			
			if (LimitLocationZ_Max < CharacterSkeletalMeshComp->GetComponentLocation ().Z)
			{
				LimitLocationZ_Max = CharacterSkeletalMeshComp->GetComponentLocation ().Z;
				LimitLocationZ_Min = LimitLocationZ_Max - LimitRange;

				// CurrentPivotLocation = LimitLocationZ_Min;
			}
			else if (CharacterSkeletalMeshComp->GetComponentLocation ().Z < LimitLocationZ_Min)
			{
				LimitLocationZ_Min = CharacterSkeletalMeshComp->GetComponentLocation ().Z;
				LimitLocationZ_Max = LimitLocationZ_Min + LimitRange;

				// CurrentPivotLocation = LimitLocationZ_Min;
			}
		}

		// CurrentPivotLocation = FMath::Max (CurrentPivotLocation, LimitLocationZ_Min);
		// CurrentPivotLocation = FMath::Min (CurrentPivotLocation, LimitLocationZ_Max);

		// UE_LOG(LogTemp, Display, TEXT("MIN: %f, Max: %f, Current: %f AA")
		// , LimitLocationZ_Min, LimitLocationZ_Max, MyActor->GetActorLocation ().Z);
		
		// FVector ResultLocation = CharacterSkeletalMeshComp->GetComponentLocation ();

		PivotLocation_Prev.X = GetComponentLocation ().X;
		PivotLocation_Prev.Y = GetComponentLocation ().Y;

		PivotLocation_Next.X = GetComponentLocation ().X;
		PivotLocation_Next.Y = GetComponentLocation ().Y;
		PivotLocation_Next.Z = LimitLocationZ_Min + LimitLocationZ_Offset;

		if (IsRawMove)
		{
			PivotLocation_Current = FMath::VInterpTo (PivotLocation_Prev, PivotLocation_Next, DeltaTime, LimitInterpSpeed_Raw);
		}
		else
		{
			PivotLocation_Current = FMath::VInterpTo (PivotLocation_Prev, PivotLocation_Next, DeltaTime, LimitInterpSpeed);
		}

		SetWorldLocation (PivotLocation_Current);

		PivotLocation_Prev = PivotLocation_Current;
	} 

}

