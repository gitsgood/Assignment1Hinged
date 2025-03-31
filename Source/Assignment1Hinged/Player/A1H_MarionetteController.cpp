// Fill out your copyright notice in the Description page of Project Settings.


#include "A1H_MarionetteController.h"
#include "EnhancedInputComponent.h"         // For UEnhancedInputComponent
#include "EnhancedInputSubsystems.h"        // For UEnhancedInputLocalPlayerSubsystem
#include "A1H_MarionetteCharacter.h"        // Include our character class header
#include "GameFramework/Character.h"        // For ACharacter base class functions
#include "Engine/LocalPlayer.h"             // For ULocalPlayer

AA1H_MarionetteController::AA1H_MarionetteController()
{
	//Constructor (feeling cute, might come back to it later)
}

void AA1H_MarionetteController::BeginPlay()
{
    Super::BeginPlay();

    // Get the Enhanced Input subsystem for the local player
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (LocalPlayer)
    {
        InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
        if (InputSubsystem)
        {
            // Add the default mapping context. Priority 0 is standard.
            // Make sure DefaultMappingContext is assigned in the Blueprint version of this controller!
            if (DefaultMappingContext)
            {
                InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
                UE_LOG(LogTemp, Log, TEXT("DefaultMappingContext added successfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("DefaultMappingContext is not set in the Player Controller Blueprint!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get Enhanced Input Subsystem!"));
        }
    }
}

void AA1H_MarionetteController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Ensure InputComponent is valid and is the correct type
    // If this fails, check Project Settings -> Input -> Default Classes -> Default Player Input Component Class
    // It should be set to 'EnhancedPlayerInputComponent'
    EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    if (!EnhancedInputComponent)
    {
        UE_LOG(LogTemp, Fatal, TEXT("Ensure Enhanced Player Input Component is set in Project Settings."));
        return; // Stop if we don't have the right input component
    }

#pragma region Bind Input Actions
    // Make sure the Action assets are assigned in the Blueprint version of this controller!

    // Move Action
    if (ActionMove)
    {
        // ETriggerEvent::Triggered fires every frame the input is active (good for movement)
        EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &AA1H_MarionetteController::HandleMove);
    }
    else UE_LOG(LogTemp, Warning, TEXT("ActionMove is not set!"));

    // Look Action
    if (ActionLook)
    {
        // ETriggerEvent::Triggered fires every frame the input is active (good for looking)
        EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &AA1H_MarionetteController::HandleLook);
    }
    else UE_LOG(LogTemp, Warning, TEXT("ActionLook is not set!"));

    // Jump Action
    if (ActionJump)
    {
        // ETriggerEvent::Started fires once when the input begins (button pressed)
        EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Started, this, &AA1H_MarionetteController::HandleJumpStarted);
        // ETriggerEvent::Completed fires once when the input ends (button released)
        EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Completed, this, &AA1H_MarionetteController::HandleJumpCompleted);
    }
    else UE_LOG(LogTemp, Warning, TEXT("ActionJump is not set!"));

    // Interact Action
    if (ActionInteract)
    {
        // ETriggerEvent::Started fires once when the input begins (usually what you want for interaction)
        EnhancedInputComponent->BindAction(ActionInteract, ETriggerEvent::Started, this, &AA1H_MarionetteController::HandleInteract);
    }
    else UE_LOG(LogTemp, Warning, TEXT("ActionInteract is not set!"));

    UE_LOG(LogTemp, Log, TEXT("Input bindings set up."));

#pragma endregion

}

#pragma region Input Handler Implementation
AA1H_MarionetteCharacter* AA1H_MarionetteController::GetControlledCharacter() const
{
    // GetPawn() returns the generic APawn the controller possesses.
    // We need to cast it to our specific character class to access its functions.
    return Cast<AA1H_MarionetteCharacter>(GetPawn());
}

void AA1H_MarionetteController::HandleMove(const FInputActionValue& Value)
{
    AA1H_MarionetteCharacter* MyCharacter = GetControlledCharacter();
    if (!MyCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("HandleMove: Controlled character is null or not AA1H_MarionetteCharacter"));
        return;
    }

    // Value is a FVector2D (because IA_Move is Axis2D)
    const FVector2D MovementVector = Value.Get<FVector2D>();

    // Tell the character to move forward/backward based on the Y component
    MyCharacter->MoveForward(MovementVector.Y);

    // Tell the character to move right/left based on the X component
    MyCharacter->MoveRight(MovementVector.X);
}

void AA1H_MarionetteController::HandleLook(const FInputActionValue& Value)
{
    AA1H_MarionetteCharacter* MyCharacter = GetControlledCharacter();
    if (!MyCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("HandleLook: Controlled character is null or not AA1H_MarionetteCharacter"));
        return;
    }

    // Value is FVector2D (because IA_Look is Axis2D)
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    // Tell the character to look right/left based on X component
    MyCharacter->LookRight(LookAxisVector.X);

    // Tell the character to look up/down based on Y component
    MyCharacter->LookUp(LookAxisVector.Y);
}

void AA1H_MarionetteController::HandleJumpStarted(const FInputActionValue& Value)
{
    AA1H_MarionetteCharacter* MyCharacter = GetControlledCharacter();
    if (!MyCharacter) return;

    // ACharacter has built-in Jump functionality
    MyCharacter->Jump();
    UE_LOG(LogTemp, Log, TEXT("Jump Started"));
}

void AA1H_MarionetteController::HandleJumpCompleted(const FInputActionValue& Value)
{
    AA1H_MarionetteCharacter* MyCharacter = GetControlledCharacter();
    if (!MyCharacter) return;

    // ACharacter has built-in Jump functionality
    MyCharacter->StopJumping();
    UE_LOG(LogTemp, Log, TEXT("Jump Completed"));
}

void AA1H_MarionetteController::HandleInteract(const FInputActionValue& Value)
{
    AA1H_MarionetteCharacter* MyCharacter = GetControlledCharacter();
    if (!MyCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("HandleInteract: Controlled character is null or not AA1H_MarionetteCharacter"));
        return;
    }

    // Value is a bool (because IA_Interact is Digital)
    // We already check ETriggerEvent::Started, but could add if (Value.Get<bool>()) for safety
    UE_LOG(LogTemp, Log, TEXT("Interact Action Triggered"));

    // Tell the character to perform its interaction check logic
    MyCharacter->PerformInteractionCheck();
}

#pragma endregion