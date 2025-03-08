// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ANIMATED_SHOT_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	FText OnFocusTarget();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void OnInteract();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void Interact(AActor* Interactor);
};
