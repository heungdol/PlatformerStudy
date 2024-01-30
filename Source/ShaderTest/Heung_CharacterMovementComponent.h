// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Heung_CharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHADERTEST_API UHeung_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY (EditAnyWhere, BlueprintReadWrite, Category = HEUNG)
	float GravityRate_Jumping = 2.5;

	UPROPERTY (EditAnyWhere, BlueprintReadWrite, Category = HEUNG)
	float GravityRate_Falling = 0.8;

	virtual float GetGravityZ() const override;

	// UFUNCTION (BlueprintImplementableEvent)
	// bool DetectUpperHandPlace (FVector& DetectLocation);

	/** Handle falling movement. */
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;
	
protected:

	virtual void BeginPlay () override;

	class ACharacter* MyCharacter;
	class AHeung_Character* MyCharacter_Heung;
};
