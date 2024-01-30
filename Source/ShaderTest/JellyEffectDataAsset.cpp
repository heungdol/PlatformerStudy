    // Fill out your copyright notice in the Description page of Project Settings.


#include "JellyEffectDataAsset.h"

int UJellyEffectDataAsset::GetJellyEffectValuesCount () const
{
    return JellyEffectValues.Num ();
}

float UJellyEffectDataAsset::GetJellyEffectTime() const
{
    return JellyEffectTime;
}

const FJellyEffectValue UJellyEffectDataAsset::GetJellyEffectValueByIndex (int index) const
{
    if (index < 0 || index >= JellyEffectValues.Num ())
    {
        return FJellyEffectValue ();
    }

    return JellyEffectValues [index];
}