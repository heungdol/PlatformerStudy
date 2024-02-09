// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class AHeung_Character;
enum class EHeung_PlatformerState_Enum : uint8;

class Heung_PlatformerState_FSM
{
public:
    Heung_PlatformerState_FSM() {}
    virtual ~Heung_PlatformerState_FSM() {}

    virtual void ResetState (AHeung_Character* Character) {}

    virtual void BeginState(AHeung_Character* Character) = 0; 
    virtual void TickState(AHeung_Character* Character, float DeltaTime, TWeakPtr<Heung_PlatformerState_FSM>& NextState) = 0; 
    virtual void ExitState(AHeung_Character* Character) = 0; 
};

class Heung_PlatformerState_FSM_Idle : public Heung_PlatformerState_FSM
{
public:
    Heung_PlatformerState_FSM_Idle 
    (
        float CrouchVelocityLength, 
        float BrakeVelocityLength,
        float BrakeDirectionDot
    )
    : CrouchVelocityLength (CrouchVelocityLength)
    , BrakeVelocityLength (BrakeVelocityLength)
    , BrakeDirectionDot (BrakeDirectionDot)
    {}

    virtual void ResetState (AHeung_Character* Character) override {}

    virtual void BeginState(AHeung_Character* Character) override;
    virtual void TickState(AHeung_Character* Character, float DeltaTime, TWeakPtr<Heung_PlatformerState_FSM>& NextState) override;
    virtual void ExitState(AHeung_Character* Character) override;

private:
    float CrouchVelocityLength;

    float BrakeVelocityLength;
    float BrakeDirectionDot;
};

class Heung_PlatformerState_FSM_Fall : public Heung_PlatformerState_FSM
{
public:
    virtual void ResetState (AHeung_Character* Character) override {}

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, TWeakPtr<Heung_PlatformerState_FSM>& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;

private:
    FRotator MeshRelativeRotation;
    FRotator MeshWorldRotation;
};

class Heung_PlatformerState_FSM_Crouch : public Heung_PlatformerState_FSM
{
public:
    virtual void ResetState (AHeung_Character* Character) override {}

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, TWeakPtr<Heung_PlatformerState_FSM>& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;
};

class Heung_PlatformerState_FSM_Slide : public Heung_PlatformerState_FSM
{
public:
    Heung_PlatformerState_FSM_Slide 
    (
        float SlideRate, 
        float SlideSpeed,
        float SlideSpeed_Jump
    ) 
    : SlideRate (SlideRate)
    , SlideSpeed (SlideSpeed) 
    , SlideSpeed_Jump (SlideSpeed_Jump)
    {}

    virtual void ResetState (AHeung_Character* Character) override 
    {
        SlideRate_Current = SlideRate;
    }

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, TWeakPtr<Heung_PlatformerState_FSM>& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;

private:
    float SlideRate_Current = 0;
    
    float SlideRate;
    float SlideSpeed;
    float SlideSpeed_Jump;
};

class Heung_PlatformerState_FSM_Stomp : public Heung_PlatformerState_FSM
{
public:
    Heung_PlatformerState_FSM_Stomp 
    (
        float StompRate_0, 
        float StompRate_1, 
        float StompRate_2, 
        float StompRate_3,
        float StompZSpeed_Fall,
        float StompZSpeed_Jump
    )
    : StompRate_0 (StompRate_0),
    StompRate_1 (StompRate_1),
    StompRate_2 (StompRate_2),
    StompRate_3 (StompRate_3),
    StompZSpeed_Fall (StompZSpeed_Fall),
    StompZSpeed_Jump (StompZSpeed_Jump)
    {}

    virtual void ResetState (AHeung_Character* Character) override 
    {
        StompRate_Current = StompRate_0;
        StomeState_Current = 0;
    }

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, TWeakPtr<Heung_PlatformerState_FSM>& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;

private:
    float StompRate_Current = 0;
    int StomeState_Current = 0;

    float StompRate_0;
    float StompRate_1;
    float StompRate_2;
    float StompRate_3;

    float StompZSpeed_Fall;
    float StompZSpeed_Jump;
};

class Heung_PlatformerState_FSM_Brake : public Heung_PlatformerState_FSM
{
public:
    Heung_PlatformerState_FSM_Brake 
    (
        float BrakeRate,
        float BrakeSpeed_Max,
        float BrakeSpeed_Min
    )
    : BrakeRate (BrakeRate)
    , BrakeSpeed_Max (BrakeSpeed_Max)
    , BrakeSpeed_Min (BrakeSpeed_Min)
    {}

    virtual void ResetState (AHeung_Character* Character) override 
    {
        BrakeRate_Current = BrakeRate;
    }

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, TWeakPtr<Heung_PlatformerState_FSM>& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;

private:
    float BrakeRate_Current = 0;

    float BrakeRate;
    float BrakeSpeed_Max;
    float BrakeSpeed_Min;

    FVector BrakeDirection;
};

class Heung_PlatformerState_FSM_Hang : public Heung_PlatformerState_FSM
{
public:
    Heung_PlatformerState_FSM_Hang (float HangJumpSpeed)//, AActor* A)
    : HangJumpSpeed (HangJumpSpeed)
    // , HangPointActor (A)
    {}

    virtual void ResetState (AHeung_Character* Character) override {}

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, TWeakPtr<Heung_PlatformerState_FSM>& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;

    // void ChangeHangPointActor (AActor *A);
private:
    float HangJumpSpeed;
    // AActor* HangPointActor;

    // FVector HangPointActor_WorldLocation;
    // FRotator HangPointActor_WorldRotation;
};