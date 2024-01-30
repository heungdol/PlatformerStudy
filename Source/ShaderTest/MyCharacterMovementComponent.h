// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHADERTEST_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY (EditAnyWhere, BlueprintReadWrite)
	float GravityRate_Jumping = 1.3;

	UPROPERTY (EditAnyWhere, BlueprintReadWrite)
	float GravityRate_Falling = 0.8;

	virtual float GetGravityZ() const override;
};
