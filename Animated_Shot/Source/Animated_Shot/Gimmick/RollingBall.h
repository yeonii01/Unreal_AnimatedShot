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
    // 나뭇가지 메쉬
    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* BranchMesh;

    // 물리 이동 적용
    UPROPERTY(VisibleAnywhere, Category = "Movement")
    UProjectileMovementComponent* MovementComponent;

    // 최초 위치 저장 (사라질 거리 계산용)
    FVector InitialLocation;
    FRotator InitialRotator;

    // 사라질 거리 (기본값: 2000)
    UPROPERTY(EditAnywhere, Category = "Settings")
    float DestroyDistance = 2000.0f;

    // 충돌 시 데미지
    UPROPERTY(EditAnywhere, Category = "Damage")
    float DamageAmount = 20.0f;

    // 충돌 시 파티클 (블루프린트에서 설정 가능)
    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* ImpactParticle;

    // 충돌 이벤트 함수
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
