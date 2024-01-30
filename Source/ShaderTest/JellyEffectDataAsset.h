// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "JellyEffectDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FJellyEffectValue
{
    GENERATED_USTRUCT_BODY()

public:
	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	float TimePoint;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	FVector ScaleRatio = FVector (1, 1, 1);
};

UCLASS()
class SHADERTEST_API UJellyEffectDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	float JellyEffectTime;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	TArray <FJellyEffectValue> JellyEffectValues;

	int GetJellyEffectValuesCount () const;

	float GetJellyEffectTime () const;

	const FJellyEffectValue GetJellyEffectValueByIndex (int index) const;
};
