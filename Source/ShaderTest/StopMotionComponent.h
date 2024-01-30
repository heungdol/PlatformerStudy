// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StopMotionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHADERTEST_API UStopMotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStopMotionComponent();

protected:
	virtual void BeginPlay() override;

	USkinnedMeshComponent* SkinnedMeshComp;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY (EditAnywhere)
	int LODCount = 4;

	UPROPERTY (EditAnywhere)
	int StopMotionRate = 4;
};
