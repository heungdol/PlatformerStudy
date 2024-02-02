// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class AHeung_Character;
enum class ECharacterPlatformingState : uint8;

class PlayerPlatformerState
{
public:
    PlayerPlatformerState() {}
    virtual ~PlayerPlatformerState() {}

    virtual void BeginState(AHeung_Character* Character) = 0; 
    virtual void TickState(AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) = 0; 
    virtual void ExitState(AHeung_Character* Character) = 0; 
};

class PlayerPlatformerState_Idle : public PlayerPlatformerState
{
public:
    PlayerPlatformerState_Idle 
    (
        float CrouchVelocityLength, 
        float BrakeVelocityLength,
        float BrakeDirectionDot
    )
    : CrouchVelocityLength (CrouchVelocityLength)
    , BrakeVelocityLength (BrakeVelocityLength)
    , BrakeDirectionDot (BrakeDirectionDot)
    {}

    virtual void BeginState(AHeung_Character* Character) override;
    virtual void TickState(AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) override;
    virtual void ExitState(AHeung_Character* Character) override;

private:
    float CrouchVelocityLength;

    float BrakeVelocityLength;
    float BrakeDirectionDot;
};

class PlayerPlatformerState_Fall : public PlayerPlatformerState
{
public:
	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;
};

class PlayerPlatformerState_Crouch : public PlayerPlatformerState
{
public:
	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;
};

class PlayerPlatformerState_Slide : public PlayerPlatformerState
{
public:
    PlayerPlatformerState_Slide 
    (
        float SlideRate, 
        float SlideSpeed
    ) 
    : SlideRate (SlideRate)
    , SlideSpeed (SlideSpeed) 
    {}

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;

private:
    float SlideRate_Current = 0;
    
    float SlideRate;
    float SlideSpeed;
};

class PlayerPlatformerState_Stomp : public PlayerPlatformerState
{
public:
    PlayerPlatformerState_Stomp 
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

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) override;
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

class PlayerPlatformerState_Brake : public PlayerPlatformerState
{
public:
    PlayerPlatformerState_Brake 
    (
        float BrakeRate,
        float BrakeSpeed_Max,
        float BrakeSpeed_Min
    )
    : BrakeRate (BrakeRate)
    , BrakeSpeed_Max (BrakeSpeed_Max)
    , BrakeSpeed_Min (BrakeSpeed_Min)
    {}

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;

private:
    float BrakeRate_Current = 0;

    float BrakeRate;
    float BrakeSpeed_Max;
    float BrakeSpeed_Min;

    FVector BrakeDirection;
};

class PlayerPlatformerState_Hang : public PlayerPlatformerState
{
public:
    PlayerPlatformerState_Hang (float HangJumpSpeed)
    : HangJumpSpeed (HangJumpSpeed)
    {}

	virtual void BeginState (AHeung_Character* Character) override;
	virtual void TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) override;
	virtual void ExitState (AHeung_Character* Character) override;

private:
    float HangJumpSpeed;
};