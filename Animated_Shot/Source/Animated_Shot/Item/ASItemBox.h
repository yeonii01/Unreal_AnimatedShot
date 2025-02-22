// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Item/ASAWeapon.h"
#include "ASItemBox.generated.h"

UCLASS()
class ANIMATED_SHOT_API AASItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASItemBox();

	FORCEINLINE class UBoxComponent* GetTrigger() { return Trigger; }
protected:
	virtual void Tick(float DeltaTime) override;  
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* BoxBodyMesh;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UParticleSystemComponent> Effect;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UASItemData> Item;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	virtual void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);

protected:
	UPROPERTY()
	bool bIsOpening = false;
	UPROPERTY()
	float CurrentRotation = 0.f;

	UPROPERTY()
	AActor* OverlapActor;
};
