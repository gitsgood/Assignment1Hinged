// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "A1H_InteractionInterface.generated.h"

// This class does not need to be modified.
// Blueprintable means Blueprints can implement this interface.
UINTERFACE(MinimalAPI, Blueprintable)
class UA1H_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface definition for actors that can be interacted with.
 */
class ASSIGNMENT1HINGED_API IA1H_InteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
	* Function called when an actor interacts with an object implementing this interface.
	* Needs to be implemented in the Actor that *has* this interface (e.g., the item).
	* BlueprintImplementableEvent means the logic *must* be implemented in Blueprint.
	* @param InteractorActor The Actor that initiated the interaction (e.g., the player character).
	*/
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
	void Interact(AActor* InteractorActor);
};
