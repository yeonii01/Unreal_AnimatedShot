// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;
};
