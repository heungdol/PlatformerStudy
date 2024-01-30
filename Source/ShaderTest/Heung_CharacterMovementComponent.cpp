// Fill out your copyright notice in the Description page of Project Settings.


#include "Heung_CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Heung_Character.h"

void UHeung_CharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay ();

    MyCharacter = Cast <ACharacter> (GetOwner ());
    MyCharacter_Heung = Cast <AHeung_Character> (GetOwner ());

    if (MyCharacter_Heung == NULL)
    {
        UE_LOG(LogTemp, Display, TEXT("HEUNG CHARA is nULL"));
    }
}

void UHeung_CharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent (DeltaTime, TickType, ThisTickFunction);

    // if (IsFalling ())
    // {
    //     if (bOrientRotationToMovement == true)
    //     {
    //         bOrientRotationToMovement = false;
    //     }
    // }  
    // else
    // {
    //     if (bOrientRotationToMovement == false)
    //     {
    //         bOrientRotationToMovement = true;
    //     }
    // }
}

float UHeung_CharacterMovementComponent::GetGravityZ() const 
{
    float Rate = 1;

    if (IsFalling ())
    {
        if (Velocity.Z > 0)
        {
            Rate = GravityRate_Jumping;
        }
        else
        {
            Rate = GravityRate_Falling;
        }
    }

    float Ret = Super::GetGravityZ() * GravityScale * Rate;
    return Ret;
}
void UHeung_CharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
    FVector SaveVelocity = Velocity;

    Super::PhysFalling (deltaTime, Iterations);

    if (IsFalling ()
    && MyCharacter != NULL && MyCharacter->GetVelocity().Z > 0
    && MyCharacter_Heung != NULL && MyCharacter_Heung->GetIsDetectCapsulePeak ())
    {
        UE_LOG(LogTemp, Display, TEXT("Detect Peak to Movement"));
        Velocity.X = SaveVelocity.X;
        Velocity.Y = SaveVelocity.Y;
    }

}
