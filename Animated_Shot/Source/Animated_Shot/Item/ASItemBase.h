// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/InteractInterface.h"
#include "ASItemBase.generated.h"

class UASItemData;

UCLASS()
class ANIMATED_SHOT_API AASItemBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UASItemData* GetItemData() const { return ItemData; }
	
	UFUNCTION(BlueprintCallable)
	void SetItemData(UASItemData* NewItemData) { ItemData = NewItemData; }

	virtual void PickUp(AActor* Actor);
	virtual void Interact_Implementation(AActor* Interactor) override;


protected:
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Type)
	FDataTableRowHandle ItemDataHandle;
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Type)
	int32 ItemQuantity = 1;

	TObjectPtr<UASItemData> ItemData = nullptr;
};
