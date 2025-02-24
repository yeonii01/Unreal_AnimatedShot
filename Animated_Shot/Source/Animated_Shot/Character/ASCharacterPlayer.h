// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/ASCharacterHUDInterface.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "PaperSpriteComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "ASCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API AASCharacterPlayer : public AASCharacterBase, public IASCharacterHUDInterface
{
	GENERATED_BODY()

public:
	AASCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Character Control Section
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	virtual void SetCharacterControlData(const class UASCharacterControlData* CharacterControlData) override;

	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> QuaterLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);
	//void QuaterLook(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	void Attack();

//Animation
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DamageMontage;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//UI Section
protected:
	virtual void SetupHUDWidget(class UASHUDWidget* InHUDWidget) override;

private:
	/** ¹Ì´Ï¸Ê¿ë ½ºÇÁ¸µ¾Ï */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* MinimapSpringArm;

	/** ¹Ì´Ï¸Ê¿ë Scene Capture */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* MinimapCapture;

	/** ¹Ì´Ï¸Ê ·»´õ Å¸°Ù */
	UPROPERTY(EditAnywhere, Category = Minimap)
	UCanvasRenderTarget2D* MinimapCanvasRenderTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	UPaperSpriteComponent* MinimapIcon;
};
