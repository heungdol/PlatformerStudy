// Fill out your copyright notice in the Description page of Project Settings.


#include "Heung_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Heung_PlatformerState_FSM.h"
#include "Heung_HangPoint.h"

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
	
    ActorComp = GetComponentByClass (UHeung_HangPoint::StaticClass ());
    HangPointComp = Cast <UHeung_HangPoint> (ActorComp);

    // PlatformingState_Next = ECharacterPlatformingState::E_IDLE;
    // CapsuleState_Current = ECharacterCapsuleState::E_STAND;

    IsDetectingHangPoint = false;
    IsDetectingCapsulePeak = false;
    IsDetectingSlidePeak = false;
    
    PlayerPlatformerState_FSM_Current = GetPlayerPlatformerState_FSM_Idle ();
    PlayerPlatformerState_FSM_Current.Pin()->ResetState (this);
    PlayerPlatformerState_FSM_Current.Pin()->BeginState (this);
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
    UpdateHangPoint (DeltaTime, IsDetectingHangPoint, HangPointLocation, HangPointDirection, HangPointRotation);//, HangPointActor);
    UpdateCapsulePeak (DeltaTime, IsDetectingCapsulePeak, DetectCapsulePeakNormal);
    UpdateSlidePeak (DeltaTime, IsDetectingSlidePeak);
    UpdateDetectDirections (DeltaTime, IsDetectingForward, IsDetectingRightward, IsDetectingBackward, IsDetectingLeftward, IsDetectingUpward, IsDetectingDownward);

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
    if (PlayerPlatformerState_FSM_Current != NULL)
    {
        PlayerPlatformerState_FSM_Current.Pin()->TickState (this, DeltaTime, PlayerPlatformerState_FSM_Next);

        if (PlayerPlatformerState_FSM_Next != NULL)
        {
            PlayerPlatformerState_FSM_Current.Pin()->ExitState (this);
            // delete PlayerPlatformerState_Current;

            PlayerPlatformerState_FSM_Current = PlayerPlatformerState_FSM_Next;
            PlayerPlatformerState_FSM_Current.Pin()->ResetState (this);
            PlayerPlatformerState_FSM_Current.Pin()->BeginState (this);

            PlayerPlatformerState_FSM_Next = NULL;
        }
    }
}

void AHeung_Character::SetCharacterMovementValuesByPlatformingState (EHeung_PlatformerState_Enum State)
{
    if (CharacterMovementComp == NULL)
    {
        return;
    }

    switch (State)
    {
        case EHeung_PlatformerState_Enum::E_IDLE:
        case EHeung_PlatformerState_Enum::E_FALL:
        case EHeung_PlatformerState_Enum::E_HANG:
        case EHeung_PlatformerState_Enum::E_STOMP_0:
        case EHeung_PlatformerState_Enum::E_STOMP_1:
        case EHeung_PlatformerState_Enum::E_STOMP_2:
        case EHeung_PlatformerState_Enum::E_BRAKE:
        case EHeung_PlatformerState_Enum::E_STOMP:

        CharacterMovementComp->MaxWalkSpeed = MaxWalkSpeed_Common;
        CharacterMovementComp->GroundFriction = GroundFriction_Common;
        CharacterMovementComp->BrakingDecelerationWalking = BrakeDecelerationWalking_Common;
        CharacterMovementComp->RotationRate = RotationRate_Common;
        
        break;

        case EHeung_PlatformerState_Enum::E_CROUCH:

        CharacterMovementComp->MaxWalkSpeed = MaxWalkSpeed_Crouch;
        CharacterMovementComp->GroundFriction = GroundFriction_Crouch;
        CharacterMovementComp->BrakingDecelerationWalking = BrakeDecelerationWalking_Crouch;
        CharacterMovementComp->RotationRate = RotationRate_Crouch;

        break;

        case EHeung_PlatformerState_Enum::E_SLIDE:

        CharacterMovementComp->MaxWalkSpeed = MaxWalkSpeed_Sliding;
        CharacterMovementComp->GroundFriction = GroundFriction_Sliding;
        CharacterMovementComp->BrakingDecelerationWalking = BrakeDecelerationWalking_Sliding;
        CharacterMovementComp->RotationRate = RotationRate_Sliding;

        break;
    }
}

void AHeung_Character::SetCapsuleHeightByPlatformingState (EHeung_PlatformerState_Enum State)
{
    if (GetCapsuleComponent () == NULL || GetMesh () == NULL)
    {
        return;
    }

    switch (State)
    {
        case EHeung_PlatformerState_Enum::E_IDLE:
        case EHeung_PlatformerState_Enum::E_HANG:
        case EHeung_PlatformerState_Enum::E_STOMP_0:
        case EHeung_PlatformerState_Enum::E_STOMP_1:
        case EHeung_PlatformerState_Enum::E_STOMP_2:
        case EHeung_PlatformerState_Enum::E_BRAKE:
        case EHeung_PlatformerState_Enum::E_STOMP:

        GetCapsuleComponent ()->SetCapsuleHalfHeight (CapsuleHeight_Common * 0.5);
        GetMesh ()->SetRelativeLocation (FVector (0, 0, SkeletalMeshHeight_Common));

        if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_STAND)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Common-CapsuleHeight_Common) * 0.5));
        }
        else if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_CROUCH)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Common-CapsuleHeight_Crouch) * 0.5));
        }
        else if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_FALL)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Common-CapsuleHeight_Falling) * 0.5));
        }

        CapsuleState_Enum_Current = EHeung_CapsuleState_Enum::E_STAND;
        
        break;

        case EHeung_PlatformerState_Enum::E_FALL:

        GetCapsuleComponent ()->SetCapsuleHalfHeight (CapsuleHeight_Falling * 0.5);
        GetMesh ()->SetRelativeLocation (FVector (0, 0, SkeletalMeshHeight_Falling));

        if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_STAND)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Falling-CapsuleHeight_Common) * 0.5));
        }
        else if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_CROUCH)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Falling-CapsuleHeight_Crouch) * 0.5));
        }
        else if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_FALL)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Falling-CapsuleHeight_Falling) * 0.5));
        }

        CapsuleState_Enum_Current = EHeung_CapsuleState_Enum::E_FALL;

        break;

        case EHeung_PlatformerState_Enum::E_CROUCH:
        case EHeung_PlatformerState_Enum::E_SLIDE:

        GetCapsuleComponent ()->SetCapsuleHalfHeight (CapsuleHeight_Crouch * 0.5);
        GetMesh ()->SetRelativeLocation (FVector (0, 0, SkeletalMeshHeight_Crouch));

        if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_STAND)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Crouch-CapsuleHeight_Common) * 0.5));
        }
        else if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_CROUCH)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Crouch-CapsuleHeight_Crouch) * 0.5));
        }
        else if (CapsuleState_Enum_Current == EHeung_CapsuleState_Enum::E_FALL)
        {
            SetActorLocation (GetActorLocation () + FVector (0, 0, (CapsuleHeight_Crouch-CapsuleHeight_Falling) * 0.5));
        }

        CapsuleState_Enum_Current = EHeung_CapsuleState_Enum::E_CROUCH;

        break;
    }
}

void AHeung_Character::Jump()
{
    if (PlatformerState_Enum_Current == EHeung_PlatformerState_Enum::E_IDLE
    || PlatformerState_Enum_Current == EHeung_PlatformerState_Enum::E_FALL)
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

TWeakPtr <Heung_PlatformerState_FSM_Idle> AHeung_Character::GetPlayerPlatformerState_FSM_Idle ()
{
    if (PlayerPlatformerState_FSM_Idle_Inst == NULL)
    {
        UE_LOG(LogTemp, Display, TEXT("Make SharedPTR: IDLE"));

        PlayerPlatformerState_FSM_Idle_Inst = MakeShared <Heung_PlatformerState_FSM_Idle>
        (
            CrouchMinVelocityLength,
            BrakeXYSpeed_Active,
            BrakeDirectionDot
        );
    }

    return PlayerPlatformerState_FSM_Idle_Inst;
}

TWeakPtr <Heung_PlatformerState_FSM_Fall> AHeung_Character::GetPlayerPlatformerState_FSM_Fall ()
{
    if (PlayerPlatformerState_FSM_Fall_Inst == NULL)
    {
        PlayerPlatformerState_FSM_Fall_Inst = MakeShared <Heung_PlatformerState_FSM_Fall> ();
    }

    return PlayerPlatformerState_FSM_Fall_Inst; 
}

TWeakPtr <Heung_PlatformerState_FSM_Crouch> AHeung_Character::GetPlayerPlatformerState_FSM_Crouch ()
{
    if (PlayerPlatformerState_FSM_Crouch_Inst == NULL)
    {
        PlayerPlatformerState_FSM_Crouch_Inst = MakeShared <Heung_PlatformerState_FSM_Crouch> (); 
    }

    return PlayerPlatformerState_FSM_Crouch_Inst;
}

TWeakPtr <Heung_PlatformerState_FSM_Slide> AHeung_Character::GetPlayerPlatformerState_FSM_Slide ()
{
    if (PlayerPlatformerState_FSM_Slide_Inst == NULL)
    {
        PlayerPlatformerState_FSM_Slide_Inst = MakeShared <Heung_PlatformerState_FSM_Slide> (SlideRate, SlideSpeed, SlideSpeed_Jump);
    }

    return PlayerPlatformerState_FSM_Slide_Inst;
}

TWeakPtr <Heung_PlatformerState_FSM_Stomp> AHeung_Character::GetPlayerPlatformerState_FSM_Stomp ()
{
    if (PlayerPlatformerState_FSM_Stomp_Inst == NULL)
    {
        PlayerPlatformerState_FSM_Stomp_Inst = MakeShared <Heung_PlatformerState_FSM_Stomp> 
        (
            StompRate_0,
            StompRate_1,
            StompRate_2,
            StompRate_3,
            StompZSpeed,
            StompZSpeed_Jump
        );
    }

    return PlayerPlatformerState_FSM_Stomp_Inst;
}

TWeakPtr <Heung_PlatformerState_FSM_Brake> AHeung_Character::GetPlayerPlatformerState_FSM_Brake ()
{
    if (PlayerPlatformerState_FSM_Brake_Inst == NULL)
    {
        PlayerPlatformerState_FSM_Brake_Inst = MakeShared <Heung_PlatformerState_FSM_Brake> 
        (
            BrakeRate,
            BrakeXYSpeed_Begin,
            BrakeXYSpeed_End
        );
    }

    return PlayerPlatformerState_FSM_Brake_Inst;
}

TWeakPtr <Heung_PlatformerState_FSM_Hang> AHeung_Character::GetPlayerPlatformerState_FSM_Hang ()
{
    if (PlayerPlatformerState_FSM_Hang_Inst == NULL)
    {
        PlayerPlatformerState_FSM_Hang_Inst = MakeShared <Heung_PlatformerState_FSM_Hang> 
        (
            HangPointDetectLength_LaunchZVelocity
        );
    }

    // if (HangPointActor != nullptr)
    // {        
    //     PlayerPlatformerStateInst_Hang->ChangeHangPointActor (HangPointActor);
    //     UE_LOG(LogTemp, Display, TEXT("HangPoint Actor: %s"), *HangPointActor->GetName());
    // }
    
    return PlayerPlatformerState_FSM_Hang_Inst;
}

void AHeung_Character::AttachHangPointCompToActor ()
{
    if (HangPointComp == nullptr 
    || HangPointActor == nullptr)
    {
        return;
    }

    HangPointComp->AttachToComponent (HangPointActor->GetRootComponent (), FAttachmentTransformRules::SnapToTargetIncludingScale);
    HangPointComp->SetWorldLocation (GetHangPointLocation_Final ());
    HangPointComp->SetWorldRotation (GetHangPointRotation ());
}

void AHeung_Character::DetachHangPointCompToActor ()
{
    if (HangPointComp == nullptr)
    {
        return;
    }

    HangPointComp->DetachFromComponent (FDetachmentTransformRules::KeepWorldTransform);
}

FTransform AHeung_Character::GetHangPointCompTransform () const
{
    FTransform Ret = FTransform ();

    if (HangPointComp != nullptr)
    {
        Ret = HangPointComp->GetComponentTransform ();
    }

    return Ret;
}