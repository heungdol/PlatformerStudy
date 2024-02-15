// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Heung_Character.generated.h"

UENUM(BlueprintType)
enum class EHeung_PlatformerState_Enum : uint8
{
	E_NULL = 0				UMETA(DisplayName = "NULL"),
	E_IDLE = 1   			UMETA(DisplayName = "IDLE"),
	E_FALL = 2   			UMETA(DisplayName = "FALL"),
	E_HANG = 3				UMETA(DisplayName = "HANG"),
	E_SLIDE = 4   			UMETA(DisplayName = "SLIDE"),
	E_STOMP_0 = 5   		UMETA(DisplayName = "STOMP_0"),
	E_STOMP_1 = 6   		UMETA(DisplayName = "STOMP_1"),
	E_STOMP_2 = 7   		UMETA(DisplayName = "STOMP_2"),
	E_CROUCH = 8   			UMETA(DisplayName = "CROUCH"),
	E_BRAKE = 9   			UMETA(DisplayName = "BRAKE"),
	E_STOMP = 10   			UMETA(DisplayName = "STOMP"),
};

UENUM(BlueprintType)
enum class EHeung_CapsuleState_Enum : uint8
{
	E_NULL = 0				UMETA(DisplayName = "NULL"),
	E_STAND = 1   			UMETA(DisplayName = "STAND"),
	E_CROUCH = 2   			UMETA(DisplayName = "CROUCH"),
	E_FALL = 3   			UMETA(DisplayName = "FALL"),
};

UCLASS()
class SHADERTEST_API AHeung_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHeung_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool CanJumpInternal_Implementation() const;

	virtual void Jump() override;

	virtual void StopJumping() override;

	void Slide ();

	void Stomp ();

	void InputAxis_Platforming_Forward (float Input);
	void InputAxis_Platforming_Right (float Input); 

	UPROPERTY (VisibleAnywhere, Category = HEUNG_COMMON)
	bool InputButton_Jump = false;
	
	UPROPERTY (VisibleAnywhere, Category = HEUNG_COMMON)
	bool InputButton_Crouch = false;

	UPROPERTY (VisibleAnywhere, Category = HEUNG_COMMON)
	float InputButtonDelay_Current = 0.1;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_COMMON)
	float InputButtonDelay = 0.2;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	FVector InputAxis_Current;

	void InputAction_Press_Platforming_Jump ();
	void InputAction_Press_Platforming_Crouch ();

	void InputAction_Release_Platforming_Jump ();
	void InputAction_Release_Platforming_Crouch ();
	
	class UCharacterMovementComponent* CharacterMovementComp;
	class UHeung_HangPoint* HangPointComp;

	// ==========================================================================================

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_PHYSICS_VALUE_WALK_BRAKE)
	float SkeletalMeshHeight_Common = -90;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_PHYSICS_VALUE_WALK_BRAKE)
	float CapsuleHeight_Common = 180;

	//

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_PHYSICS_VALUE_WALK_BRAKE)
	float SkeletalMeshHeight_Falling = -90;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_PHYSICS_VALUE_WALK_BRAKE)
	float CapsuleHeight_Falling = 150;

	//

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_WALK_BRAKE)
	float MaxWalkSpeed_Common = 600;
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_WALK_BRAKE)
	float GroundFriction_Common = 8;
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_WALK_BRAKE)
	FRotator RotationRate_Common = FRotator (0, 800, 0);

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_WALK_BRAKE)
	float MaxAcceleration_Common = 2000;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_WALK_BRAKE)
	float BrakeDecelerationWalking_Common = 4000;

	
	//
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_PHYSICS_VALUE_CROUCH_SLIDE)
	float SkeletalMeshHeight_Crouch = -20; 

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_PHYSICS_VALUE_CROUCH_SLIDE)
	float CapsuleHeight_Crouch = 100; 

	//

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	float MaxWalkSpeed_Crouch = 0; 

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	float GroundFriction_Crouch = 8; 

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	float MaxAcceleration_Crouch = 0;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	float BrakeDecelerationWalking_Crouch = 4000; 

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	FRotator RotationRate_Crouch = FRotator (0, 0, 0);

	//

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	float MaxWalkSpeed_Sliding = 900; 

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	float GroundFriction_Sliding = 0; 

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	float MaxAcceleration_Sliding = 30000;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	float BrakeDecelerationWalking_Sliding = 0; 

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_MOVEMENT_VALUE_CROUCH_SLIDE)
	FRotator RotationRate_Sliding = FRotator (0, 100, 0);

	//

	UPROPERTY (VisibleAnywhere, Category = HEUNG_COMMON)
	EHeung_CapsuleState_Enum CapsuleState_Enum_Current = EHeung_CapsuleState_Enum::E_STAND;

	UPROPERTY (VisibleAnywhere, Category = HEUNG_COMMON)
	EHeung_PlatformerState_Enum PlatformerState_Enum_Current = EHeung_PlatformerState_Enum::E_IDLE;

	TWeakPtr <class Heung_PlatformerState_FSM> PlayerPlatformerState_FSM_Current;
	TWeakPtr <Heung_PlatformerState_FSM> PlayerPlatformerState_FSM_Next;

	TSharedPtr <class Heung_PlatformerState_FSM_Idle> PlayerPlatformerState_FSM_Idle_Inst;
	TSharedPtr <class Heung_PlatformerState_FSM_Fall> PlayerPlatformerState_FSM_Fall_Inst;
	TSharedPtr <class Heung_PlatformerState_FSM_Crouch> PlayerPlatformerState_FSM_Crouch_Inst;
	TSharedPtr <class Heung_PlatformerState_FSM_Slide> PlayerPlatformerState_FSM_Slide_Inst;
	TSharedPtr <class Heung_PlatformerState_FSM_Stomp> PlayerPlatformerState_FSM_Stomp_Inst;
	TSharedPtr <class Heung_PlatformerState_FSM_Brake> PlayerPlatformerState_FSM_Brake_Inst;
	TSharedPtr <class Heung_PlatformerState_FSM_Hang> PlayerPlatformerState_FSM_Hang_Inst;

	//

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingHangPoint;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingSlidePeak;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingCapsulePeak;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingForward;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingRightward;
	
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingLeftward;
	
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingBackward;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingUpward;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	bool IsDetectingDownward;

	// UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	// FVector CurrentFloorDirection_Front;

	// UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	// FVector CurrentFloorDirection_Right;
	
	// UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	// FVector CurrentFloorDirection_Up;

	//

	UPROPERTY (VisibleAnywhere, BlueprintReadonly, Category = HEUNG_HANG)
	FVector HangPointLocation;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_HANG)
	FVector HangPointDirection;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_HANG)
	FRotator HangPointRotation;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_HANG)
	FVector HangPointLocation_Final;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite, Category = HEUNG_HANG)
	AActor* HangPointActor;

	//

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_COMMON)
	FVector DetectCapsulePeakNormal;

	// ==========================================================================================

	UPROPERTY (VisibleAnywhere, Category = HEUNG_KOYOTEJUMP)
	bool IsInAir = false;

	UPROPERTY (VisibleAnywhere, Category = HEUNG_KOYOTEJUMP)
	bool IsAbleToKoyoteJump = false;

	UPROPERTY (VisibleAnywhere, Category = HEUNG_KOYOTEJUMP)
	bool IsInputedKoyoteJump = false;

	UPROPERTY (VisibleAnywhere, Category = HEUNG_KOYOTEJUMP)
	float KoyoteCurrentTime = 0;

	UPROPERTY (EditAnyWhere, BlueprintReadWrite, Category = HEUNG_KOYOTEJUMP)
	float KoyoteTime = 0.3;

	void UpdateKoyoteTime (float DeltaTime);

	// ==========================================================================================

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float DirectionDetect_Height_Chest = 50;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float DirectionDetect_Length = 23;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float DirectionDetect_Length_UpDown = 100;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float DirectionDetect_Radius = 5;

	//

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float HangPointDetectLength_Forward = 50;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float HangPointDetectLength_Radius = 10;
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float HangPointDetectLength_Hook = 30;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float HangPointDetectLength_Height_Peak = 180;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float HangPointDetectLength_Height_Offset = -180;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float HangPointDetectLength_LaunchZVelocity = 1000;

	//

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float SlideDetect_Radius = 10;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float SlideDetect_Height = 0;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float SlideDetect_Length = 60;

	//

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float PeakDetect_Radius = 25;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float PeakDetect_Height = 0;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float PeakDetect_Length = 100;

	//

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_DETECT)
	float DetectCapsulePeakNormalDot = 0.5; 
	
	//

	UFUNCTION (BlueprintImplementableEvent)
	void UpdateHangPoint (float DeltaTime, bool& IsDetect, FVector& HandLocation, FVector& HandDirection, FRotator& HandRotation);//, AActor* HPA);
	
	UFUNCTION (BlueprintImplementableEvent)
	void UpdateCapsulePeak (float DeltaTime, bool& IsDetect, FVector& Normal);
	
	UFUNCTION (BlueprintImplementableEvent)
	void UpdateSlidePeak (float DeltaTime, bool& IsDetect);
	
	UFUNCTION (BlueprintImplementableEvent)
	void UpdateDetectDirections (float DeltaTime, bool& Forward, bool& Rightward, bool& Backward, bool& Leftward, bool& Upward, bool& Downward);
	
	// UFUNCTION (BlueprintImplementableEvent)
	// void UpdateCurrentFloorDirections (float DeltaTime, FVector& FrontVector, FVector& RightVector, FVector& UpVector);
	
	// ==========================================================================================

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_BRAKE)
	float BrakeDirectionDot = -0.75;
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_BRAKE)
	float BrakeXYSpeed_Active = 100;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_BRAKE)
	float BrakeXYSpeed_Begin = 300;
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_BRAKE)
	float BrakeXYSpeed_End = 10;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_BRAKE)
	float BrakeRate = 0.5;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = HEUNG_BRAKE)
	FVector BrakeDirection = FVector (1, 0, 0);

	// ==========================================================================================

	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_SLIDE)
	float SlideRate = 0.5;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_SLIDE)
	float SlideRate_Slope = 0.1;
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_SLIDE)
	float SlideAngleDot = 0.6;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_SLIDE)
	float SlideSpeed = 1000;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_SLIDE)
	float SlideSpeed_Jump = 1600;
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_SLIDE)
	float SlideSpeed_Jump_Edge = 1000;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_SLIDE)
	float SlideSpeed_Down = -200;

	// ==========================================================================================

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_CROUCH)
	float CrouchMinVelocityLength = 10;
	
	// ==========================================================================================

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_STOMP)
	float StompRate_0 = 0.3;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_STOMP)
	float StompRate_1 = 10;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_STOMP)
	float StompRate_2 = 0.2;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_STOMP)
	float StompRate_3 = 0.3;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_STOMP)
	float StompZSpeed = 2000;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = HEUNG_STOMP)
	float StompZSpeed_Jump = 2000;
	
	// ==========================================================================================
	
	UFUNCTION ()
	void UpdateCharacterPlatformingState (float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ======================================================================
	UFUNCTION ()
	void ResetInputButtonDelay ()
	{
		InputButtonDelay_Current = InputButtonDelay;
	}

	UFUNCTION ()
	bool GetIsInputButtonAble () const
	{
		return (InputButtonDelay_Current < 0);
	}

	// ======================================================================
	
	UFUNCTION (BlueprintPure)
	bool GetIsDetectCapsulePeak () const
	{
		return IsDetectingCapsulePeak && (FVector::DotProduct (FVector (0, 0, -1), DetectCapsulePeakNormal) > DetectCapsulePeakNormalDot);
	}

	UFUNCTION (BlueprintPure)
	bool GetIsDetectSlidePeak () const
	{
		return IsDetectingSlidePeak;
	}

	UFUNCTION (BlueprintPure)
	bool GetIsDetectHangPoint () const
	{
		return IsDetectingHangPoint;
	}

	UFUNCTION (BlueprintPure)
	bool GetIsDetectForward () const
	{
		return IsDetectingForward;
	}
	
	UFUNCTION (BlueprintPure)
	bool GetIsDetectBackward () const
	{
		return IsDetectingBackward;
	}

	UFUNCTION (BlueprintPure)
	bool GetIsDetectLeftward () const
	{
		return IsDetectingLeftward;
	}

	UFUNCTION (BlueprintPure)
	bool GetIsDetectRightward () const
	{
		return IsDetectingRightward;
	}

	UFUNCTION (BlueprintPure)
	bool GetIsDetectUpward () const
	{
		return IsDetectingUpward;
	}

	UFUNCTION (BlueprintPure)
	bool GetIsDetectDownward () const
	{
		return IsDetectingDownward;
	}

	// =====================================================================

	// UFUNCTION (BlueprintPure)
	// FVector GetCurrentFloorDirection_Front () const
	// {
	// 	return CurrentFloorDirection_Front;
	// }

	// UFUNCTION (BlueprintPure)
	// FVector GetCurrentFloorDirection_Right () const
	// {
	// 	return CurrentFloorDirection_Right;
	// }

	// UFUNCTION (BlueprintPure)
	// FVector GetCurrentFloorDirection_Up () const
	// {
	// 	return CurrentFloorDirection_Up;
	// }

	// ======================================================================

	UFUNCTION (BlueprintPure)
	FVector GetVelocityDirection () const
	{
		FVector Ret = GetVelocity ();
		Ret = Ret.GetSafeNormal2D ();

		if (Ret.Length () < 0.000001)
		{
			Ret = GetActorForwardVector ();
		}

		return Ret;
	}

	UFUNCTION (BlueprintPure)
	FVector GetInputAxisDirection () const
	{
		FRotator ControlRotation = GetControlRotation ();
		ControlRotation.Pitch = 0;
		ControlRotation.Roll = 0;

		FVector Ret = FRotationMatrix (ControlRotation).GetUnitAxis (EAxis::X) * InputAxis_Current.X;
		Ret += FRotationMatrix (ControlRotation).GetUnitAxis (EAxis::Y) * InputAxis_Current.Y;
		Ret = Ret.GetSafeNormal ();

		if (Ret.Length () < 0.000001)
		{
			Ret = GetActorForwardVector ();
		}

		return Ret;
	}

	UFUNCTION (BlueprintPure)
	FVector GetHangPointDetectDirection () const
	{
		FVector Ret = GetInputAxisDirection ();

		if (Ret.Length () < 0.000001)
		{
			Ret = GetVelocityDirection ();
		}

		return Ret;
	}

	UFUNCTION (BlueprintPure)
	FVector GetHangPointLocation_Final ()
	{
		HangPointLocation_Final = HangPointLocation;
        HangPointLocation_Final -= HangPointDirection * HangPointDetectLength_Forward;
        HangPointLocation_Final -= FVector (0, 0, HangPointDetectLength_Height_Peak);
	
		return HangPointLocation_Final;
	}

	UFUNCTION (BlueprintPure)
	AActor* GetHangPointActor ()
	{
		return HangPointActor;
	}

	UFUNCTION (BlueprintPure)
	FRotator GetHangPointRotation ()
	{
		return HangPointRotation;
	}

	UFUNCTION (BlueprintImplementableEvent)
	FVector GetDirectionByCurrentFloor (const FVector& InputDirection);
	
	UFUNCTION (BlueprintImplementableEvent)
	FVector GetCurrentFloorNormal ();

	// ======================================================================

	UFUNCTION ()
	bool GetInputButton_Jump () const
	{
		return InputButton_Jump;
	}

	UFUNCTION ()
	bool GetInputButton_Crouch () const
	{
		return InputButton_Crouch;
	}

	UFUNCTION ()
	void SetInputButton_Jump (bool b)
	{
		InputButton_Jump = b;
	}

	UFUNCTION ()
	void SetInputButton_Crouch (bool b)
	{
		InputButton_Crouch = b;
	}

	// ======================================================================

	UFUNCTION ()
	void SetCharacterPlatformingState (EHeung_PlatformerState_Enum E)
	{
		PlatformerState_Enum_Current = E;
	}

	UFUNCTION ()
	EHeung_PlatformerState_Enum GetCharacterPlatformingState () const
	{
		return PlatformerState_Enum_Current;
	}

	// ======================================================================

	UFUNCTION ()
	void SetCharacterMovementValuesByPlatformingState (EHeung_PlatformerState_Enum State);
	
	UFUNCTION ()
	void SetCapsuleHeightByPlatformingState (EHeung_PlatformerState_Enum State);

	// ===================================================================

	UFUNCTION ()
	void AttachHangPointCompToActor ();
	
	UFUNCTION ()
	void DetachHangPointCompToActor ();

	UFUNCTION ()
	FTransform GetHangPointCompTransform () const;

	// ===================================================================

	TWeakPtr <Heung_PlatformerState_FSM_Idle> GetPlayerPlatformerState_FSM_Idle ();
	TWeakPtr <Heung_PlatformerState_FSM_Fall> GetPlayerPlatformerState_FSM_Fall ();
	TWeakPtr <Heung_PlatformerState_FSM_Crouch> GetPlayerPlatformerState_FSM_Crouch ();
	TWeakPtr <Heung_PlatformerState_FSM_Slide> GetPlayerPlatformerState_FSM_Slide ();
	TWeakPtr <Heung_PlatformerState_FSM_Stomp> GetPlayerPlatformerState_FSM_Stomp ();
	TWeakPtr <Heung_PlatformerState_FSM_Brake> GetPlayerPlatformerState_FSM_Brake ();
	TWeakPtr <Heung_PlatformerState_FSM_Hang> GetPlayerPlatformerState_FSM_Hang ();
};
