// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Heung_JellyEffectComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHADERTEST_API UHeung_JellyEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHeung_JellyEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void JellyEffectByGravity (float DelatTime);
	
	ACharacter* MyActor;
	USkeletalMeshComponent* MySkeletalMeshComp;
	class UCharacterMovementComponent* MyCharacterMovementComp;

	void JellyEffectNextStep ();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY (VisibleAnywhere, Category = JellyEffect_Debug)
	FVector Velocity;

	UPROPERTY (VisibleAnywhere, Category = JellyEffect_Debug)
	FVector StartScale;

	// UPROPERTY (EditAnywhere, Category = JellyEffect_InAir)
	// float Velocity_Z_InterpSpeed = 100;

	UPROPERTY (EditAnywhere, Category = JellyEffect_InAir)
	float Velocity_Z_Max = 500;

	UPROPERTY (EditAnywhere, Category = JellyEffect_InAir)
	float Velocity_Z_Min = 0;

	UPROPERTY (EditAnyWhere, Category = JellyEffect_InAir)
	FVector Scale_Z_Max = FVector (0.9, 0.9, 1.25);
	
	UPROPERTY (EditAnyWhere, Category = JellyEffect_InAir)
	FVector Scale_Z_Min = FVector (1.0, 1.0, 1.0);

	UPROPERTY (EditAnyWhere, Category = JellyEffect_InAir)
	FVector Scale_Z_Min_InAir = FVector (1.1, 1.1, 0.9);

	UPROPERTY (EditAnyWhere, Category = JellyEffect_InAir)
	FVector Velocity_Z_CurrentScaleRatio;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = JellyEffect_Play)
	class UHeung_JellyEffectDataAsset* JellyEffectData_Jump;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = JellyEffect_Play)
	UHeung_JellyEffectDataAsset* JellyEffectData_Land;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = JellyEffect_Common)
	float Scale_InterpSpeed = 1;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = JellyEffect_Debug)
	UHeung_JellyEffectDataAsset* JellyEffectData_Current;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = JellyEffect_Debug)
	int JellyEffectData_CurrentIndex;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = JellyEffect_Debug)
	FVector JellyEffectData_CurrentScaleRatio = FVector (1, 1, 1);

	UFUNCTION (BlueprintCallable)
	void PlayJellyEffectData (UHeung_JellyEffectDataAsset* Data);
	
	UFUNCTION (BlueprintCallable)
	void StopJellyEffectData ();

	UPROPERTY ()
	FTimerHandle TimerHandle;

};

