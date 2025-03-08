// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASItemBase.h"
#include "../Components/InventoryComponent.h"

// Sets default values
AASItemBase::AASItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASItemBase::PickUp(AActor* Actor)
{
	if (!IsValid(Actor))
		return;

	UInventoryComponent* Inventory = Actor->GetComponentByClass<UInventoryComponent>();

	if (!IsValid(Inventory))
		return;

	if (Inventory->PickupItem(this))
		Destroy();
}

void AASItemBase::Interact_Implementation(AActor* Interactor)
{
	PickUp(Interactor);
}
