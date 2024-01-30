// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Heung_SpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHADERTEST_API UHeung_SpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
protected:
	/** Updates the desired arm location, calling BlendLocations to do the actual blending if a trace is done */
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;

};
