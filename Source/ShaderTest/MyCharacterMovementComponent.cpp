// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterMovementComponent.h"


float UMyCharacterMovementComponent::GetGravityZ() const 
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

    UE_LOG(LogTemp, Display, TEXT("Gravity: %f"), Ret);

    return Ret;
}
