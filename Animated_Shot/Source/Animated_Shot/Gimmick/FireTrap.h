// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "FireTrap.generated.h"

UCLASS()
class ANIMATED_SHOT_API AFireTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Components")
    UParticleSystemComponent* FireParticle;

    UPROPERTY(BlueprintReadWrite, Category = "Components")
    UBoxComponent* DamageZone;

    UPROPERTY(EditAnywhere, Category = "Flamethrower")
    float FireActiveTime; 

    UPROPERTY(EditAnywhere, Category = "Flamethrower")
    float FireInactiveTime; 

    UPROPERTY(EditAnywhere, Category = "Flamethrower")
    float Damage; 

    bool bIsFlameActive;

    float TimeSinceLastToggle;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};
