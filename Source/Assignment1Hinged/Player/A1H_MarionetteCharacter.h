// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "A1H_MarionetteCharacter.generated.h"

// Forward declarations - Tells the compiler these classes exist without needing the full header yet
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext; // We need this later for the Controller, but good practice to declare types needed by functions
class UInputAction;

UCLASS()
class ASSIGNMENT1HINGED_API AA1H_MarionetteCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties (Default constructor)
	AA1H_MarionetteCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Components
	// Spring Arm: Attaches camera to character but keeps it at a distance, handling collisions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	// Camera: The player's viewpoint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

#pragma endregion

#pragma region Interaction
	// How far the character can reach to interact
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractionDistance = 350.0f;


	// Performs a trace to find interactable objects in front of the camera 
	void PerformInteractionCheck();

#pragma endregion

#pragma region Movement Callbacks (Called BY the PlayerController)
	// Handles moving forward/backward. Value is -1.0 to +1.0
	void MoveForward(float Value);

	// Handles moving right/left. Value is -1.0 to +1.0
	void MoveRight(float Value);

	// Handles looking up/down. Value is mouse delta or joystick input
	void LookUp(float Value);

	// Handles looking left/right. Value is mouse delta or joystick input
	void LookRight(float Value);

	// Note: Jump is already handled by ACharacter's built-in Jump() and StopJumping() functions

#pragma endregion



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Grant friendship to the controller class so it can call protected/private members if needed
	// In this case, I made the movement functions protected, so the controller needs access.
	friend class AA1H_MarionetteController;

	// A helper function to get the Camera Component (useful for the controller or other classes)
	UCameraComponent* GetCameraComponent() const { return CameraComp; }
};
