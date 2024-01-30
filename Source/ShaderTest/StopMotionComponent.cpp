#include "StopMotionComponent.h"
#include "Components/SkinnedMeshComponent.h"

UStopMotionComponent::UStopMotionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UStopMotionComponent::BeginPlay()
{
	Super::BeginPlay();

	UActorComponent* ActorComp;
	if (GetOwner() != NULL)
	{
		ActorComp = GetOwner()->GetComponentByClass (USkinnedMeshComponent::StaticClass ());
		SkinnedMeshComp = Cast <USkinnedMeshComponent> (ActorComp);
		
		if (SkinnedMeshComp != NULL)
		{
			SkinnedMeshComp->bEnableUpdateRateOptimizations = true;

			SkinnedMeshComp->AnimUpdateRateParams->bShouldUseLodMap = true;
			// SkinnedMeshComp->AnimUpdateRateParams->bShouldUseMinLod = true;

			SkinnedMeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Empty ();

			for (int lod = 0; lod < LODCount; lod++)
			{
				SkinnedMeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(lod, StopMotionRate);
			}
		}
	}
}

void UStopMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Display, TEXT("Component Tick: %i"), SkinnedMeshComp->AnimUpdateRateParams->bSkipUpdate);

	// if (SkinnedMeshComp != NULL)
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("UpdateRate: %d"), SkinnedMeshComp->AnimUpdateRateParams->SkippedUpdateFrames);
	// }
}

