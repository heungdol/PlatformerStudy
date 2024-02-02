// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPlatformerState.h"
#include "Heung_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// ============================================================================================================

void PlayerPlatformerState_Idle::BeginState (AHeung_Character* Character)
{
    // UE_LOG(LogTemp, Display, TEXT("State Begin: IDLE"));

    if (Character == NULL)
    {
        return;
    }

    Character->ResetInputButtonDelay ();

    Character->SetCharacterPlatformingState (ECharacterPlatformingState::E_IDLE);
    Character->SetCharacterMovementValuesByPlatformingState (ECharacterPlatformingState::E_IDLE);
    Character->SetCapsuleHeightByPlatformingState (ECharacterPlatformingState::E_IDLE);

    if (Character->GetController () != NULL)
    {
        Character->GetController ()->ResetIgnoreMoveInput ();
    }

    if (Character->GetCharacterMovement() != NULL)
    {
        Character->GetCharacterMovement()->SetActive (true);
    }

    if (Character->GetCharacterMovement() != NULL)
    {
        Character->GetCharacterMovement()->bOrientRotationToMovement = true;
    }
}

void PlayerPlatformerState_Idle::TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState)
{
    if (Character == NULL)
    {
        return;
    }

    // UE_LOG(LogTemp, Display, TEXT("State Tick: IDLE"));

    if (Character->GetCharacterMovement ()->IsFalling () == true)
    {
        NextState = Character->GetPlayerPlatformerState_Fall ();
    }
    else if (Character->GetIsInputButtonAble () && Character->GetInputButton_Crouch () == true)
    {
        if (Character->GetVelocity ().Length () < CrouchVelocityLength)
        {
            NextState = Character->GetPlayerPlatformerState_Crouch ();
        }
        else
        {
            NextState = Character->GetPlayerPlatformerState_Slide ();
        }
    }
    else if ((FVector::DotProduct (Character->GetVelocityDirection (), Character->GetInputAxisDirection()) < BrakeDirectionDot)
    && Character->GetVelocity ().Length() > BrakeVelocityLength)
    {
        NextState = Character->GetPlayerPlatformerState_Brake ();
    }
}

void PlayerPlatformerState_Idle::ExitState (AHeung_Character* Character)
{
    // UE_LOG(LogTemp, Display, TEXT("State Exit: IDLE"));

}

// ============================================================================================================

void PlayerPlatformerState_Fall::BeginState (AHeung_Character* Character) 
{
    if (Character == NULL)
    {
        return;
    }

    // UE_LOG(LogTemp, Display, TEXT("State Begin: FALL"));	

    Character->ResetInputButtonDelay ();

    Character->SetCharacterPlatformingState (ECharacterPlatformingState::E_FALLING);
    Character->SetCharacterMovementValuesByPlatformingState (ECharacterPlatformingState::E_FALLING);
    Character->SetCapsuleHeightByPlatformingState (ECharacterPlatformingState::E_FALLING);

    if (Character->GetController () != NULL)
    {
        Character->GetController ()->ResetIgnoreMoveInput ();
    }

    if (Character->GetCharacterMovement() != NULL)
    {
        Character->GetCharacterMovement()->SetActive (true);
    }

    if (Character->GetCharacterMovement() != NULL)
    {
        Character->GetCharacterMovement()->bOrientRotationToMovement = true;
    }

}

void PlayerPlatformerState_Fall::TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState) 
{
    if (Character == NULL)
    {
        return;
    }

    // UE_LOG(LogTemp, Display, TEXT("State Tick: FALL"));

    if (Character->GetCharacterMovement ()->IsFalling () == false)
    {
        NextState = Character->GetPlayerPlatformerState_Idle ();
    }
    else if (Character->GetIsInputButtonAble () && Character->GetInputButton_Crouch ())
    {
        NextState = Character->GetPlayerPlatformerState_Stomp ();
    }
    else if (Character->GetCharacterMovement () != NULL 
    && Character->GetCharacterMovement ()->IsFalling () 
    && Character->GetVelocity ().Z < 0
    && Character->GetIsDetectHangPoint () == true)
    {
        bool IsPassed_Loc = false;
        bool IsPassed_Dot = false;

        // // 추가 조건들
        if (FVector::Distance (Character->GetHangPointLocation_Final(), FVector (0, 0, 0)) > 1)
        {
            IsPassed_Loc = true;
        }

        if (FVector::DotProduct (Character->GetInputAxisDirection (), Character->GetHangPointDetectDirection ()) > 0.5)
        {
            IsPassed_Dot = true;
        }

        if (IsPassed_Loc && IsPassed_Dot)
        {
            NextState = Character->GetPlayerPlatformerState_Hang ();
        }
    }
}

void PlayerPlatformerState_Fall::ExitState (AHeung_Character* Character) 
{
    // UE_LOG(LogTemp, Display, TEXT("State Exit: FALL"));		
}

// ============================================================================================================

void PlayerPlatformerState_Crouch::BeginState(AHeung_Character* Character)
{
    if (Character == NULL)
    {
        return;
    }

    Character->ResetInputButtonDelay ();

    Character->SetCharacterPlatformingState (ECharacterPlatformingState::E_CROUCH);
    Character->SetCharacterMovementValuesByPlatformingState (ECharacterPlatformingState::E_CROUCH);
    Character->SetCapsuleHeightByPlatformingState (ECharacterPlatformingState::E_CROUCH);

    if (Character->GetController () != NULL)
    {
        Character->GetController ()->SetIgnoreMoveInput (true);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->SetActive (true);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->ResetMoveState ();
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->bOrientRotationToMovement = true;
    }
}

void PlayerPlatformerState_Crouch::TickState(AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState)
{
    if (Character == NULL)
    {
        return;
    }

    if (Character->GetCharacterMovement ()->IsFalling () == true)
    {
        NextState = Character->GetPlayerPlatformerState_Fall ();
    }
    else if (Character->GetIsInputButtonAble () &&  Character->GetInputButton_Crouch () == false)
    {
        NextState = Character->GetPlayerPlatformerState_Idle ();
    }
}

void PlayerPlatformerState_Crouch::ExitState(AHeung_Character* Character)
{
    
}

// ============================================================================================================


void PlayerPlatformerState_Slide::BeginState(AHeung_Character* Character)
{
    if (Character == NULL)
    {
        return;
    }

    Character->ResetInputButtonDelay ();

    Character->SetCharacterPlatformingState (ECharacterPlatformingState::E_SLIDING);
    Character->SetCharacterMovementValuesByPlatformingState (ECharacterPlatformingState::E_SLIDING);
    Character->SetCapsuleHeightByPlatformingState (ECharacterPlatformingState::E_SLIDING);

    Character->SetInputButton_Crouch (false);

    if (Character->GetController () != NULL)
    {
        Character->GetController ()->ResetIgnoreMoveInput ();
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->SetActive (true);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->ResetMoveState ();
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->bOrientRotationToMovement = true;
    }

    Character->SetActorRotation (Character->GetInputAxisDirection ().Rotation ());

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->LaunchCharacter (Character->GetActorForwardVector () * SlideSpeed, true, false);
    }

    SlideRate_Current = SlideRate;
}

void PlayerPlatformerState_Slide::TickState(AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState)
{
    if (Character == NULL)
    {
        return;
    }

    if (SlideRate_Current < 0 && Character->GetIsDetectSlidePeak () == false)
    {
        NextState = Character->GetPlayerPlatformerState_Idle ();
    }
    // else if (Character->GetCharacterMovement () != NULL
    // && Character->GetCharacterMovement ()->IsFalling () == true)
    // {
    //     NextState = Character->GetPlayerPlatformerState_Fall ();
    // }

    SlideRate_Current -= DeltaTime;
}

void PlayerPlatformerState_Slide::ExitState(AHeung_Character* Character)
{
    
}

// ============================================================================================================


void PlayerPlatformerState_Stomp::BeginState(AHeung_Character* Character)
{
    if (Character == NULL)
    {
        return;
    }

    Character->ResetInputButtonDelay ();

    Character->SetCharacterPlatformingState (ECharacterPlatformingState::E_STOMP);
    Character->SetCharacterMovementValuesByPlatformingState (ECharacterPlatformingState::E_STOMP);
    Character->SetCapsuleHeightByPlatformingState (ECharacterPlatformingState::E_STOMP);

    Character->SetInputButton_Crouch (false);

    if (Character->GetController () != NULL)
    {
        Character->GetController ()->SetIgnoreMoveInput (true);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->SetActive (false);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->ResetMoveState ();
    }

    StompRate_Current = StompRate_0;
}

void PlayerPlatformerState_Stomp::TickState(AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState)
{
    if (Character == NULL)
    {
        return;
    }

    if (StompRate_Current < 0)
    {
        StomeState_Current += 1;

        switch (StomeState_Current)
        {
            case 0:

            break;

            case 1:

            StompRate_Current = StompRate_1;

            if (Character->GetCharacterMovement () != NULL)
            {
                Character->GetCharacterMovement ()->SetActive (true);
            }

            break;

            case 2:

            StompRate_Current = StompRate_2;

            break;

            case 3:

            StompRate_Current = StompRate_3;

            break;

            case 4:

            NextState = Character->GetPlayerPlatformerState_Idle ();

            break;
        }
    }

    switch (StomeState_Current)
    {
        case 0:

        break;

        case 1:

        if (Character->GetCharacterMovement () != NULL)
        {
            Character->GetCharacterMovement ()->ResetMoveState ();
        }

        Character->SetActorLocation (Character->GetActorLocation () + FVector (0, 0, -1) * StompZSpeed_Fall * DeltaTime, true);

        if (Character->GetCharacterMovement () != NULL && Character->GetCharacterMovement ()->IsFalling () == false)
        {
            StompRate_Current = 0;
        }

        break;

        case 2:

        // Just wating...

        break;

        case 3:

        if (Character->GetIsInputButtonAble () && Character->GetInputButton_Jump ())
        {
            if (Character->GetCharacterMovement () != NULL)
            {
                Character->GetCharacterMovement ()->SetActive (true);
            }

            if (Character->GetController () != NULL)
            {
                Character->GetController ()->ResetIgnoreMoveInput ();
            }

            Character->LaunchCharacter (FVector (0, 0, StompZSpeed_Jump), false, false);

            NextState = Character->GetPlayerPlatformerState_Fall ();
        }

        break;
    }

    StompRate_Current -= DeltaTime;
}

void PlayerPlatformerState_Stomp::ExitState(AHeung_Character* Character)
{
    
}

// ============================================================================================================

void PlayerPlatformerState_Brake::BeginState (AHeung_Character* Character)
{
    if (Character == NULL)
    {
        return;
    }

    if (Character->GetController () != NULL)
    {
        Character->GetController ()->SetIgnoreMoveInput (true);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->SetActive (true);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->ResetMoveState ();
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->bOrientRotationToMovement = true;
    }

    BrakeDirection = Character->GetVelocityDirection ();

    Character->SetActorRotation (BrakeDirection.Rotation ());

    BrakeRate_Current = BrakeRate;
}

void PlayerPlatformerState_Brake::TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState)
{
    if (Character == NULL)
    {
        return;
    }

    float BrakeSpeed = FMath::Lerp (BrakeSpeed_Min, BrakeSpeed_Max, BrakeRate_Current / BrakeRate); 
    Character->SetActorLocation (Character->GetActorLocation () + BrakeDirection * BrakeSpeed * DeltaTime, true);

    if (Character->GetCharacterMovement () != NULL && Character->GetCharacterMovement ()->IsFalling ())
    {
        NextState = Character->GetPlayerPlatformerState_Fall ();
    }
    else if (BrakeRate_Current < 0)
    {
        NextState = Character->GetPlayerPlatformerState_Idle ();
    }

    BrakeRate_Current -= DeltaTime;
}

void PlayerPlatformerState_Brake::ExitState (AHeung_Character* Character)
{

}

// ============================================================================================================

void PlayerPlatformerState_Hang::BeginState (AHeung_Character* Character)
{
    if (Character == NULL)
    {
        return;
    }

    if (Character->GetController () != NULL)
    {
        Character->GetController ()->SetIgnoreMoveInput (true);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->SetActive (false);
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->ResetMoveState ();
    }

    if (Character->GetCharacterMovement () != NULL)
    {
        Character->GetCharacterMovement ()->bOrientRotationToMovement = true;
    }

    Character->SetActorLocation (Character->GetHangPointLocation_Final ());
    Character->SetActorRotation (Character->GetHangPointRotation ());
}

void PlayerPlatformerState_Hang::TickState (AHeung_Character* Character, float DeltaTime, PlayerPlatformerState*& NextState)
{
    if (Character == NULL)
    {
        return;
    }

    if (Character->GetIsInputButtonAble () && Character->GetInputButton_Crouch () == true)
    {
        Character->SetInputButton_Crouch (false);

        if (Character->GetController () != NULL)
        {
            Character->GetController ()->ResetIgnoreMoveInput ();
        }

        if (Character->GetCharacterMovement() != NULL)
        {
            Character->GetCharacterMovement()->SetActive (true);
        }

        NextState = Character->GetPlayerPlatformerState_Fall ();
    }
    else if (Character->GetIsInputButtonAble () && Character->GetInputButton_Jump () == true)
    {
        if (Character->GetController () != NULL)
        {
            Character->GetController ()->ResetIgnoreMoveInput ();
        }

        if (Character->GetCharacterMovement() != NULL)
        {
            Character->GetCharacterMovement()->SetActive (true);
        }

        Character->LaunchCharacter (FVector (0, 0, HangJumpSpeed), false, false);

        NextState = Character->GetPlayerPlatformerState_Fall ();
    }
}

void PlayerPlatformerState_Hang::ExitState (AHeung_Character* Character)
{
    
}