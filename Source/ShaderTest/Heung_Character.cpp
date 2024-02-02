// Fill out your copyright notice in the Description page of Project Settings.


#include "Heung_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerPlatformerState.h"

// Sets default values
AHeung_Character::AHeung_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHeung_Character::BeginPlay()
{
	Super::BeginPlay();

	UActorComponent* ActorComp = GetComponentByClass (UCharacterMovementComponent::StaticClass ());
    CharacterMovementComp = Cast <UCharacterMovementComponent> (ActorComp);

    // PlatformingState_Next = ECharacterPlatformingState::E_IDLE;
    // CapsuleState_Current = ECharacterCapsuleState::E_STAND;

    IsDetectingHangPoint = false;
    IsDetectingCapsulePeak = false;
    IsDetectingSlidePeak = false;
    
    PlayerPlatformerState_Current = GetPlayerPlatformerState_Idle ();
    PlayerPlatformerState_Current->BeginState (this);
}

// Called every frame
void AHeung_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (InputButtonDelay_Current > 0)
    {
        InputButtonDelay_Current -= DeltaTime;
    }

	UpdateKoyoteTime (DeltaTime);
    UpdateHangPoint (DeltaTime, IsDetectingHangPoint, HangPointLocation, HangPointDirection, HangPointRotation);
    UpdateCapsulePeak (DeltaTime, IsDetectingCapsulePeak, DetectCapsulePeakNormal);
    UpdateSlidePeak (DeltaTime, IsDetectingSlidePeak);
    
    UpdateCharacterPlatformingState (DeltaTime);
}

// Called to bind functionality to input
void AHeung_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction (TEXT ("Jump"), IE_Pressed, this, &AHeung_Character::InputAction_Press_Platforming_Jump);
    PlayerInputComponent->BindAction (TEXT ("Crouch"), IE_Pressed, this, &AHeung_Character::InputAction_Press_Platforming_Crouch);
    
    PlayerInputComponent->BindAction (TEXT ("Jump"), IE_Released, this, &AHeung_Character::InputAction_Release_Platforming_Jump);
    PlayerInputComponent->BindAction (TEXT ("Crouch"), IE_Released, this, &AHeung_Character::InputAction_Release_Platforming_Crouch);

    PlayerInputComponent->BindAxis (TEXT ("MoveX"), this, &AHeung_Character::InputAxis_Platforming_Forward);
    PlayerInputComponent->BindAxis (TEXT ("MoveY"), this, &AHeung_Character::InputAxis_Platforming_Right);
}

bool AHeung_Character::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation () || IsInputedKoyoteJump;
}

void AHeung_Character::UpdateKoyoteTime(float DeltaTime)
{
    if (CharacterMovementComp == NULL)
    {
        IsAbleToKoyoteJump = false;

        return;
    }

    if (CharacterMovementComp->IsFalling () == false)
    {
        IsInAir = false;

        IsAbleToKoyoteJump = false;
        IsInputedKoyoteJump = false;

        return;
    }

    if (IsInAir == false)
    {
        IsInAir = true;

        if (CharacterMovementComp->Velocity.Z <= 5.0)
        {
            IsAbleToKoyoteJump = true;
            IsInputedKoyoteJump = false;

            KoyoteCurrentTime = 0;
        }
        else
        {
            IsAbleToKoyoteJump = false;
        }
    }

    if (IsAbleToKoyoteJump == true)
    {
        KoyoteCurrentTime += DeltaTime;
        if (KoyoteCurrentTime > KoyoteTime)
        {
            IsAbleToKoyoteJump = false;
        }
    }
    else if (IsInputedKoyoteJump == true)
    {
        KoyoteCurrentTime += DeltaTime;

        if (KoyoteCurrentTime > JumpMaxHoldTime)
        {
            IsInputedKoyoteJump = false;
        }
    }
}

void AHeung_Character::UpdateCharacterPlatformingState(float DeltaTime)
{
    if (PlayerPlatformerState_Current != NULL)
    {
        PlayerPlatformerState_Current->TickState (this, DeltaTime, PlayerPlatformerState_Next);

        if (PlayerPlatformerState_Next != NULL)
        {
            PlayerPlatformerState_Current->ExitState (this);
            delete PlayerPlatformerState_Current;

            PlayerPlatformerState_Current = PlayerPlatformerState_Next;
            PlayerPlatformerState_Current->BeginState (this);

            PlayerPlatformerState_Next = NULL;
        }
    }
}

void AHeung_Character::SetCharacterMovementValuesByPlatformingState (ECharacterPlatformingState State)
{
    if (CharacterMovementComp == NULL)
    {
        return;
    }

    switch (State)
    {
        case ECharacterPlatformingState::E_IDLE:
        case ECharacterPlatformingState::E_FALLING:
        case ECharacterPlatformingState::E_HANGING:
        case ECharacterPlatformingState::E_STOMP_0:
        case ECharacterPlatformingState::E_STOMP_1:
        case ECharacterPlatformingState::E_STOMP_2:
        case ECharacterPlatformingState::E_BRAKE:

        CharacterMovementComp->MaxWalkSpeed = MaxWalkSpeed_Common;
        CharacterMovementComp->GroundFriction = GroundFriction_Common;
        CharacterMovementComp->BrakingDecelerationWalking = BrakeDecelerationWalking_Common;
        CharacterMovementComp->RotationRate = RotationRate_Common;
        
        break;

        case ECharacterPlatformingState::E_CROUCH:

        CharacterMovementComp->MaxWalkSpeed = MaxWalkSpeed_Crouch;
        CharacterMovementComp->GroundFriction = GroundFriction_Crouch;
        CharacterMovementComp->BrakingDecelerationWalking = BrakeDecelerationWalking_Crouch;
        CharacterMovementComp->RotationRate = RotationRate_Crouch;

        break;

        case ECharacterPlatformingState::E_SLIDING:

        CharacterMovementComp->MaxWalkSpeed = MaxWalkSpeed_Sliding;
        CharacterMovementComp->GroundFriction = GroundFriction_Sliding;
        CharacterMovementComp->BrakingDecelerationWalking = BrakeDecelerationWalking_Sliding;
        CharacterMovementComp->RotationRate = RotationRate_Sliding;

        break;
    }
}

void AHeung_Character::SetCapsuleHeightByPlatformingState (ECharacterPlatformingState State)
{
    if (GetCapsuleComponent () == NULL || GetMesh () == NULL)
    {
        return;
    }

    switch (State)
    {
        case ECharacterPlatformingState::E_IDLE:
        case ECharacterPlatformingState::E_HANGING:
        case ECharacterPlatformingState::E_STOMP_0:
        case ECharacterPlatformingState::E_STOMP_1:
        case ECharacterPlatformingState::E_STOMP_2:
        case ECharacterPlatformingState::E_BRAKE:

        GetCapsuleComponent ()->SetCapsuleHalfHeight (CapsuleHeight_Common * 0.5);
        GetMesh ()->SetRelativeLocation (FVector (0, 0, SkeletalMeshHeight_Common));

        if (CapsuleState_Current == ECharacterCapsuleState::E_STAND)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Common-CapsuleHeight_Common) * 0.5));
        }
        else if (CapsuleState_Current == ECharacterCapsuleState::E_CROUCH)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Common-CapsuleHeight_Crouch) * 0.5));
        }
        else if (CapsuleState_Current == ECharacterCapsuleState::E_FALLING)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Common-CapsuleHeight_Falling) * 0.5));
        }

        CapsuleState_Current = ECharacterCapsuleState::E_STAND;
        
        break;

        case ECharacterPlatformingState::E_FALLING:

        GetCapsuleComponent ()->SetCapsuleHalfHeight (CapsuleHeight_Falling * 0.5);
        GetMesh ()->SetRelativeLocation (FVector (0, 0, SkeletalMeshHeight_Falling));

        if (CapsuleState_Current == ECharacterCapsuleState::E_STAND)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Falling-CapsuleHeight_Common) * 0.5));
        }
        else if (CapsuleState_Current == ECharacterCapsuleState::E_CROUCH)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Falling-CapsuleHeight_Crouch) * 0.5));
        }
        else if (CapsuleState_Current == ECharacterCapsuleState::E_FALLING)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Falling-CapsuleHeight_Falling) * 0.5));
        }

        CapsuleState_Current = ECharacterCapsuleState::E_FALLING;

        break;

        case ECharacterPlatformingState::E_CROUCH:
        case ECharacterPlatformingState::E_SLIDING:

        GetCapsuleComponent ()->SetCapsuleHalfHeight (CapsuleHeight_Crouch * 0.5);
        GetMesh ()->SetRelativeLocation (FVector (0, 0, SkeletalMeshHeight_Crouch));

        if (CapsuleState_Current == ECharacterCapsuleState::E_STAND)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Crouch-CapsuleHeight_Common) * 0.5));
        }
        else if (CapsuleState_Current == ECharacterCapsuleState::E_CROUCH)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Crouch-CapsuleHeight_Crouch) * 0.5));
        }
        else if (CapsuleState_Current == ECharacterCapsuleState::E_FALLING)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Crouch-CapsuleHeight_Falling) * 0.5));
        }

        CapsuleState_Current = ECharacterCapsuleState::E_CROUCH;

        break;
    }
}

void AHeung_Character::Jump()
{
    if (PlatformingStateEnum_Current == ECharacterPlatformingState::E_IDLE
    || PlatformingStateEnum_Current == ECharacterPlatformingState::E_FALLING)
    {
        Super::Jump ();

        if (IsAbleToKoyoteJump)
        {
            IsAbleToKoyoteJump = false;
            IsInputedKoyoteJump = true;

            KoyoteCurrentTime = 0;
        }
    }
}

void AHeung_Character::StopJumping()
{
    Super::StopJumping ();

    IsInputedKoyoteJump = false;
}

void AHeung_Character::InputAxis_Platforming_Forward (float Input)
{
    // if (Input < 0)
    // {
    //     InputAxis_Current.X = -1;
    // }
    // else if (Input > 0)
    // {
    //     InputAxis_Current.X = 1;
    // }
    // else
    // {
    //     InputAxis_Current.X = 0;
    // }

    InputAxis_Current.X = Input;
}

void AHeung_Character::InputAxis_Platforming_Right (float Input)
{
    // if (Input < 0)
    // {
    //     InputAxis_Current.Y = -1;
    // }
    // else if (Input > 0)
    // {
    //     InputAxis_Current.Y = 1;
    // }
    // else
    // {
    //     InputAxis_Current.Y = 0;
    // }

    InputAxis_Current.Y = Input;
}

void AHeung_Character::InputAction_Press_Platforming_Jump ()
{
    InputButton_Jump = true;
}

void AHeung_Character::InputAction_Press_Platforming_Crouch ()
{
    InputButton_Crouch = true;
}

void AHeung_Character::InputAction_Release_Platforming_Jump ()
{
    InputButton_Jump = false;
}

void AHeung_Character::InputAction_Release_Platforming_Crouch ()
{
    InputButton_Crouch = false;
}

PlayerPlatformerState_Idle* AHeung_Character::GetPlayerPlatformerState_Idle ()
{
    return new PlayerPlatformerState_Idle 
    (
        CrouchMinVelocityLength,
        BrakeXYSpeed_Active,
        BrakeDirectionDot
    );
}

PlayerPlatformerState_Fall* AHeung_Character::GetPlayerPlatformerState_Fall ()
{
    return new PlayerPlatformerState_Fall ();
}

PlayerPlatformerState_Crouch* AHeung_Character::GetPlayerPlatformerState_Crouch ()
{
    return new PlayerPlatformerState_Crouch ();
}

PlayerPlatformerState_Slide* AHeung_Character::GetPlayerPlatformerState_Slide ()
{
    return new PlayerPlatformerState_Slide (SlideRate, SlideSpeed);
}

PlayerPlatformerState_Stomp* AHeung_Character::GetPlayerPlatformerState_Stomp ()
{
    return new PlayerPlatformerState_Stomp 
    (
        StompRate_0,
        StompRate_1,
        StompRate_2,
        StompRate_3,
        StompZSpeed,
        StompZSpeed_Jump
    );
}

PlayerPlatformerState_Brake* AHeung_Character::GetPlayerPlatformerState_Brake ()
{
    return new PlayerPlatformerState_Brake 
    (
        BrakeRate,
        BrakeXYSpeed_Begin,
        BrakeXYSpeed_End
    );
}

PlayerPlatformerState_Hang* AHeung_Character::GetPlayerPlatformerState_Hang ()
{
    return new PlayerPlatformerState_Hang 
    (
        HangPointDetectLength_LaunchZVelocity
    );
}