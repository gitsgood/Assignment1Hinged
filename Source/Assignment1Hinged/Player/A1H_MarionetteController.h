// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h" // Required for Enhanced Input action handler parameters
#include "A1H_MarionetteController.generated.h"


// Forward Declarations
class UInputMappingContext;
class UInputAction;
class AA1H_MarionetteCharacter; // Forward declare the character class
class UEnhancedInputLocalPlayerSubsystem; // Forward declare subsystem
class UEnhancedInputComponent; // Forward declare input component type

/**
 * 
 */
UCLASS()
class ASSIGNMENT1HINGED_API AA1H_MarionetteController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//Constructor
	AA1H_MarionetteController();

protected:
	//Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	//Called to bind functionality to input.
	virtual void SetupInputComponent() override;

#pragma region Input Assets
    // These properties will allow us to assign our IA and IMC assets in the Blueprint subclass

    // The Input Mapping Context to use for default controls.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
    TObjectPtr<UInputMappingContext> DefaultMappingContext; // TObjectPtr is safer than raw pointers

    // Input Action for moving forward/backward/left/right.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
    TObjectPtr<UInputAction> ActionMove;

    // Input Action for looking around.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
    TObjectPtr<UInputAction> ActionLook;

    // Input Action for jumping.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
    TObjectPtr<UInputAction> ActionJump;

    // Input Action for interacting.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
    TObjectPtr<UInputAction> ActionInteract;


#pragma endregion

private:
#pragma region Input Handling Functions
    // These functions are directly bound to the Input Actions

    // Called when the Move input action is triggered.
    void HandleMove(const FInputActionValue& Value);

    // Called when the Look input action is triggered.
    void HandleLook(const FInputActionValue& Value);

    // Called when the Jump input action is started.
    void HandleJumpStarted(const FInputActionValue& Value);

    // Called when the Jump input action is completed (button released).
    void HandleJumpCompleted(const FInputActionValue& Value);

    // Called when the Interact input action is triggered (usually on start).
    void HandleInteract(const FInputActionValue& Value);

#pragma endregion

#pragma region Helper Functions
    // Utility function to get the controlled character cast to our specific class.
    AA1H_MarionetteCharacter* GetControlledCharacter() const;

    // Cached pointers to avoid frequent casting or lookups
    UPROPERTY() // UPROPERTY prevents garbage collection
        TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;

    UPROPERTY()
    TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem = nullptr;

#pragma endregion

};
