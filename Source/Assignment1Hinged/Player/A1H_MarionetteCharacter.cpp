// Fill out your copyright notice in the Description page of Project Settings.


#include "A1H_MarionetteCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h" // Needed for movement settings
#include "Components/CapsuleComponent.h" // Might be needed for interaction trace ignore
#include "Kismet/KismetSystemLibrary.h" // For LineTrace
#include "A1H_InteractionInterface.h" // Include the interface header
#include "DrawDebugHelpers.h" // Optional: For visualizing the trace

// Sets default values
AA1H_MarionetteCharacter::AA1H_MarionetteCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// Chris: I wanna turn it off and find out ok...
	PrimaryActorTick.bCanEverTick = false;

#pragma region Configure Character Movement
    // Don't rotate the character mesh with the camera pitch/roll
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    // Rotate the character mesh towards the direction they are moving
    GetCharacterMovement()->bOrientRotationToMovement = true;
    // How fast the character turns to face movement direction
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

#pragma endregion

#pragma region Create And Setup Components
    // Create Spring Arm
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    // Attach to the character's root
    SpringArmComp->SetupAttachment(RootComponent); 
    // How far back the camera sits
    SpringArmComp->TargetArmLength = 300.0f; 
    // Make the Spring Arm rotate with the Controller's rotation (which handles mouse/stick look)
    SpringArmComp->bUsePawnControlRotation = true;

#pragma endregion

#pragma region Create Camera
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    // Attach the camera to the *end* of the spring arm
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    // Camera should NOT rotate relative to the spring arm (the arm already follows controller rotation)
    CameraComp->bUsePawnControlRotation = false;

#pragma endregion

    /* Note: Assigning Skeletal Mesh and Animation Blueprint is best done in a
     * Blueprint subclass of this C++ class to avoid hardcoding asset paths in C++. */

}

#pragma region Movement
void AA1H_MarionetteCharacter::MoveForward(float Value)
{
    // Check if we have a controller and the input value is significant
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // Find out which way is "forward" based on the controller's rotation (yaw only)
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get the forward direction vector from this rotation
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // Add movement input in that direction
        AddMovementInput(Direction, Value);
    }
}

void AA1H_MarionetteCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // Find out which way is "right" based on the controller's rotation (yaw only)
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get the right direction vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Add movement input in that direction
        AddMovementInput(Direction, Value);
    }
}

void AA1H_MarionetteCharacter::LookUp(float Value)
{
    if (Value != 0.0f)
    {
        // Add Pitch input (up/down look) - Unreal negates this internally usually for mouse, so I negated it back ffs.
        AddControllerPitchInput(-Value);
    }
}

void AA1H_MarionetteCharacter::LookRight(float Value)
{
    if (Value != 0.0f)
    {
        // Add Yaw input (left/right look)
        AddControllerYawInput(Value);
    }
}

#pragma endregion

#pragma region Interaction
void AA1H_MarionetteCharacter::PerformInteractionCheck()
{
    if (!CameraComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("AA1H_MarionetteCharacter::PerformInteractionCheck - CameraComp is null!"));
        return;
    }

    // Get character's location and camera's and forward direction
    FVector TraceStart = RootComponent->GetComponentLocation();
    FVector TraceEnd = TraceStart + (CameraComp->GetForwardVector() * InteractionDistance);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    // Make sure the trace ignores the character itself
    QueryParams.AddIgnoredActor(this);

    // Perform the line trace (AKA raycast)
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,        // Output hit result
        TraceStart,       // Start location
        TraceEnd,         // End location
        ECC_Visibility,   // Trace channel (things that block visibility)
        QueryParams       // Additional parameters (like ignoring self)
    );

    // Optional: Draw a debug line to see the trace in the game world
    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 1.0f);

    // Check if we hit something
    if (bHit && HitResult.GetActor()) // Also check if the hit actor is valid
    {
        AActor* HitActor = HitResult.GetActor();
        // Check if the hit actor *implements* our InteractionInterface
        if (HitActor->Implements<UA1H_InteractionInterface>())
        {
            // Call the interface function on the HitActor.
            // The 'Execute_' prefix is required for calling interface functions from C++.
            // We pass 'this' (the character) as the InteractorActor.
            IA1H_InteractionInterface::Execute_Interact(HitActor, this);

            UE_LOG(LogTemp, Log, TEXT("Interacted with: %s"), *HitActor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Hit actor %s does not implement InteractionInterface."), *HitActor->GetName());
        }
    }
}

#pragma endregion

// Called when the game starts or when spawned
void AA1H_MarionetteCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA1H_MarionetteCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AA1H_MarionetteCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

