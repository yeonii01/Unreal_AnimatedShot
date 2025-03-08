// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/ASCharacterPlayer.h"
#include "ASTrap.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class ANIMATED_SHOT_API AASTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
    UFUNCTION()
    void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trap")
    void ActivateTrap();

protected:
    TObjectPtr<AASCharacterPlayer> OverlapActor = nullptr;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
    TObjectPtr<UNiagaraComponent> NiagaraEffect = nullptr;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
    TObjectPtr<UNiagaraComponent> CircleEffect = nullptr;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TObjectPtr<USphereComponent> TriggerZone = nullptr;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trap")
    float TrapDamage = 20.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trap")
    float ChargeSpeed = 20.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trap")
    float CurrentCharge = 0.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trap")
    bool bPlayerOnTrigger = false;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trap")
    bool bTrapActivated = false;
};
