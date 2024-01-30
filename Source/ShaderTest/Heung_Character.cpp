// Fill out your copyright notice in the Description page of Project Settings.


#include "Heung_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

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

    PlatformingState_Next = ECharacterPlatformingState::E_IDLE;
    CapsuleState_Current = ECharacterCapsuleState::E_STAND;

    IsDetectingHangPoint = false;
    IsDetectingCapsulePeak = false;
    IsDetectingSlidePeak = false;
	
}

// Called every frame
void AHeung_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
    // Renew Stage
    if (PlatformingState_Next != ECharacterPlatformingState::E_NULL
    && PlatformingState_Next != PlatformingState_Current)
    {
        switch (PlatformingState_Next)
        {
            case ECharacterPlatformingState::E_IDLE:

            UE_LOG(LogTemp, Display, TEXT("IDLE"));

            if (GetController () != NULL)
            {
                GetController ()->ResetIgnoreMoveInput ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = true;
            }

            break;
            
            case ECharacterPlatformingState::E_FALLING:

            if (GetController () != NULL)
            {
                GetController ()->ResetIgnoreMoveInput ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = false;
            }
            
            break;

            case ECharacterPlatformingState::E_HANGING:

            if (GetController () != NULL)
            {
                GetController ()->SetIgnoreMoveInput (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (false);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->ResetMoveState ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = true;
            }

            SetActorLocation (HangPointLocation_Final);
            SetActorRotation (HangPointRotation);

            break;

            case ECharacterPlatformingState::E_STOMP_0:

            if (GetController () != NULL)
            {
                GetController ()->SetIgnoreMoveInput (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (false);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->ResetMoveState ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = true;
            }

            StompRate_Current = StompRate_0;

            break;

            case ECharacterPlatformingState::E_STOMP_1:

            if (GetController () != NULL)
            {
                GetController ()->SetIgnoreMoveInput (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = true;
            }

            break;

            case ECharacterPlatformingState::E_STOMP_2:

            if (GetController () != NULL)
            {
                GetController ()->SetIgnoreMoveInput (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = true;
            }

            StompRate_Current = StompRate_2;

            break;

            case ECharacterPlatformingState::E_BRAKE:

            if (GetController () != NULL)
            {
                GetController ()->SetIgnoreMoveInput (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->ResetMoveState ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = true;
            }

            SetActorRotation (BrakeDirection.Rotation ());

            // if (CharacterMovementComp != NULL)
            // {
            //     LaunchCharacter (GetActorForwardVector () * BrakeXYSpeed_Max, true, false);
            // }

            BrakeRate_Current = BrakeRate;

            BrakeXYSpeed_Begin = GetVelocity ().Length ();
            BrakeXYSpeed_Begin = FMath::Min (BrakeXYSpeed_Begin, BrakeXYSpeed_Max);

            break;

            case ECharacterPlatformingState::E_CROUCH:
            
            if (GetController () != NULL)
            {
                GetController ()->SetIgnoreMoveInput (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->ResetMoveState ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = true;
            }

            break;

            case ECharacterPlatformingState::E_SLIDING:
            
            if (GetController () != NULL)
            {
                GetController ()->ResetIgnoreMoveInput ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->ResetMoveState ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->bOrientRotationToMovement = true;
            }

            if (CharacterMovementComp != NULL)
            {
                LaunchCharacter (GetActorForwardVector () * MaxWalkSpeed_Sliding, true, false);
            }

            SlidingRate_Current = SlidingRate;

            break;
        }

        PlatformingState_Current = PlatformingState_Next;
        PlatformingState_Next = ECharacterPlatformingState::E_NULL;

        InputButtonDelay_Current = InputButtonDelay;

        SetCharacterMovementValuesByPlatformingState (PlatformingState_Current);
        SetCapsuleHeightByPlatformingState (PlatformingState_Current);
    }

    // Update by State
    switch (PlatformingState_Current)
    {
        case ECharacterPlatformingState::E_IDLE:

        if (InputButton_Crouch)
        {
            if (GetVelocity ().Length () < CrouchMinVelocityLength)
            {
                PlatformingState_Next = ECharacterPlatformingState::E_CROUCH;
            }
            else
            {
                // 하드하게 설정
                InputButton_Crouch = false;

                PlatformingState_Next = ECharacterPlatformingState::E_SLIDING;
            }
        }
        else if (CharacterMovementComp != NULL && CharacterMovementComp->IsFalling ())
        {
            PlatformingState_Next = ECharacterPlatformingState::E_FALLING;
        }
        else
        {
            FVector CurrentVelocityNormal = GetVelocity ().GetSafeNormal ();
            FVector CurrentInputNormal = GetInputAxisDirection ();

            // FVector CurrentInputNormal = FRotationMatrix (GetControlRotation ()).GetScaledAxis (EAxis::X) * InputAxis_Current.X;
            // CurrentInputNormal += FRotationMatrix (GetControlRotation ()).GetScaledAxis (EAxis::Y) * InputAxis_Current.Y;
            // CurrentInputNormal = CurrentInputNormal.GetSafeNormal ();

            if (FVector::DotProduct (CurrentVelocityNormal, CurrentInputNormal) < BrakeDirectionDot
            && GetVelocity ().Length () > BrakeXYSpeed_Active)
            {
                UE_LOG(LogTemp, Display, TEXT("BRAKE"));

                BrakeDirection = CurrentVelocityNormal;
                PlatformingState_Next = ECharacterPlatformingState::E_BRAKE;
            }
        }
        // else if (FVector::DotProduct (CurrentVelocityNormal, CurrentInputNormal) < BrakeDirectionDot)
        // {
        //     UE_LOG(LogTemp, Display, TEXT("BRAKE"));
        //     // PlatformingState_Next = ECharacterPlatformingState::E_BRAKE;
        // }

        break;
        
        case ECharacterPlatformingState::E_FALLING:

        if (InputButtonDelay_Current < 0 && InputButton_Crouch)
        {
            // 하드하게 설정
            InputButton_Crouch = false;

            PlatformingState_Next = ECharacterPlatformingState::E_STOMP_0;
        }
        else if (CharacterMovementComp != NULL && !CharacterMovementComp->IsFalling ())
        {
            PlatformingState_Next = ECharacterPlatformingState::E_IDLE;
        }
        else if (CharacterMovementComp != NULL 
        && CharacterMovementComp->IsFalling () && GetVelocity ().Z < 0
        && IsDetectingHangPoint == true)
        {
            HangPointLocation_Final = HangPointLocation;
            HangPointLocation_Final -= HangPointDirection * HangPointDetectLength_Forward;
            HangPointLocation_Final -= FVector (0, 0, HangPointDetectLength_Height_Peak);

            bool IsPassed_Loc = false;
            bool IsPassed_Dot = false;

            // // 추가 조건들
            if (FVector::Distance (HangPointLocation_Final, FVector (0, 0, 0)) > 1)
            {
                IsPassed_Loc = true;
            }

            if (FVector::DotProduct (GetInputAxisDirection (), HangPointDirection) > 0.5)
            {
                IsPassed_Dot = true;
            }

            if (IsPassed_Loc && IsPassed_Dot)
            {
                PlatformingState_Next = ECharacterPlatformingState::E_HANGING;
            }
        }

        break;

        case ECharacterPlatformingState::E_HANGING:

        if (InputButtonDelay_Current < 0 && InputButton_Crouch)
        {
            // 하드하게 설정
            InputButton_Crouch = false;

            PlatformingState_Next = ECharacterPlatformingState::E_FALLING;

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }
        }
        else if (InputButtonDelay_Current < 0 && InputButton_Jump)
        {
            PlatformingState_Next = ECharacterPlatformingState::E_FALLING;

            if (GetController () != NULL)
            {
                GetController ()->ResetIgnoreMoveInput ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            LaunchCharacter (FVector (0, 0, HangPointDetectLength_LaunchZVelocity), false, false);
        }

        break;

        case ECharacterPlatformingState::E_STOMP_0:

        StompRate_Current -= DeltaTime;

        if (StompRate_Current < 0)
        {
            PlatformingState_Next = ECharacterPlatformingState::E_STOMP_1;
        }

        break;

        case ECharacterPlatformingState::E_STOMP_1:

        if (CharacterMovementComp != NULL)
        {
            CharacterMovementComp->ResetMoveState ();
        }

        SetActorLocation (GetActorLocation () + FVector (0, 0, -1) * StompZSpeed * DeltaTime, true);

        if (CharacterMovementComp != NULL && CharacterMovementComp->IsFalling () == false)
        {
            PlatformingState_Next = ECharacterPlatformingState::E_STOMP_2;
        }

        break;

        case ECharacterPlatformingState::E_STOMP_2:

        StompRate_Current -= DeltaTime;

        if (StompRate_Current < 0)
        {
            PlatformingState_Next = ECharacterPlatformingState::E_IDLE;
        }

        if (InputButtonDelay_Current < 0 && InputButton_Jump)
        {
            PlatformingState_Next = ECharacterPlatformingState::E_FALLING;

            if (GetController () != NULL)
            {
                GetController ()->ResetIgnoreMoveInput ();
            }

            if (CharacterMovementComp != NULL)
            {
                CharacterMovementComp->SetActive (true);
            }

            LaunchCharacter (FVector (0, 0, StompZSpeed_Jump), false, false);
        }

        break;

        case ECharacterPlatformingState::E_BRAKE:

        {
            float BrakeSpeed = FMath::Lerp (BrakeXYSpeed_Min, BrakeXYSpeed_Begin, BrakeRate_Current / BrakeRate); 
            SetActorLocation (GetActorLocation () + BrakeDirection * BrakeSpeed * DeltaTime, true);
        }

        if (CharacterMovementComp != NULL && CharacterMovementComp->IsFalling ())
        {
            PlatformingState_Next = ECharacterPlatformingState::E_FALLING;
        }
        else if (BrakeRate_Current < 0)
        {
            PlatformingState_Next = ECharacterPlatformingState::E_IDLE;
        }

        BrakeRate_Current -= DeltaTime;

        break;

        case ECharacterPlatformingState::E_CROUCH:

        if (InputButton_Crouch == false)
        {
            PlatformingState_Next = ECharacterPlatformingState::E_IDLE;
        }

        break;

        case ECharacterPlatformingState::E_SLIDING:

        SlidingRate_Current -= DeltaTime;

        if (SlidingRate_Current < 0 && IsDetectingSlidePeak == false)
        {
            PlatformingState_Next = ECharacterPlatformingState::E_IDLE;
        }

        if (CharacterMovementComp != NULL)
        {
            CharacterMovementComp->ResetMoveState ();
        }

        break;
    }

    if (InputButtonDelay_Current > 0)
    {
        InputButtonDelay_Current -= DeltaTime;
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

        
        // CharacterMovementComp->MaxWalkSpeed = MaxWalkSpeed_Common;
        // CharacterMovementComp->GroundFriction = GroundFriction_Common;
        // CharacterMovementComp->BrakingDecelerationWalking = BrakeDecelerationWalking_Brake;
        // CharacterMovementComp->RotationRate = RotationRate_Brake;
        
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
    if (PlatformingState_Current == ECharacterPlatformingState::E_IDLE
    || PlatformingState_Current == ECharacterPlatformingState::E_FALLING)
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