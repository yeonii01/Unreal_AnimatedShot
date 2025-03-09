// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "RollingBall.generated.h"

UCLASS()
class ANIMATED_SHOT_API ARollingBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARollingBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
    virtual void Tick(float DeltaTime) override;

    void ResetBranch();
private:
    // �������� �޽�
    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* BranchMesh;

    // ���� �̵� ����
    UPROPERTY(VisibleAnywhere, Category = "Movement")
    UProjectileMovementComponent* MovementComponent;

    // ���� ��ġ ���� (����� �Ÿ� ����)
    FVector InitialLocation;
    FRotator InitialRotator;

    // ����� �Ÿ� (�⺻��: 2000)
    UPROPERTY(EditAnywhere, Category = "Settings")
    float DestroyDistance = 2000.0f;

    // �浹 �� ������
    UPROPERTY(EditAnywhere, Category = "Damage")
    float DamageAmount = 20.0f;

    // �浹 �� ��ƼŬ (�������Ʈ���� ���� ����)
    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* ImpactParticle;

    // �浹 �̺�Ʈ �Լ�
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
