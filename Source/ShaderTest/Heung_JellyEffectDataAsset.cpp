    // Fill out your copyright notice in the Description page of Project Settings.


#include "Heung_JellyEffectDataAsset.h"

int UHeung_JellyEffectDataAsset::GetJellyEffectValuesCount () const
{
    return JellyEffectValues.Num ();
}

float UHeung_JellyEffectDataAsset::GetJellyEffectTime() const
{
    return JellyEffectTime;
}

const FJellyEffectValue UHeung_JellyEffectDataAsset::GetJellyEffectValueByIndex (int index) const
{
    if (index < 0 || index >= JellyEffectValues.Num ())
    {
        return FJellyEffectValue ();
    }

    return JellyEffectValues [index];
}