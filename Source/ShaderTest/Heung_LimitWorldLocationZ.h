// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Heung_LimitWorldLocationZ.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHADERTEST_API UHeung_LimitWorldLocationZ : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHeung_LimitWorldLocationZ();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AActor* MyActor;
	class ACharacter* MyCharacter;
	class UCharacterMovementComponent* CharacterMovementComp;
	class USkeletalMeshComponent* CharacterSkeletalMeshComp;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LimitRange = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LimitLocationZ_Offset = 80;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LimitInterpSpeed_Raw = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LimitInterpSpeed = 10;

	UPROPERTY (VisibleAnywhere)
	FVector PivotLocation_Prev;

	UPROPERTY (VisibleAnywhere)
	FVector PivotLocation_Current;

	UPROPERTY (VisibleAnywhere)
	FVector PivotLocation_Next;

	UPROPERTY (VisibleAnywhere)
	float LimitLocationZ_Max;

	UPROPERTY (VisibleAnywhere)
	float LimitLocationZ_Min;		
};
