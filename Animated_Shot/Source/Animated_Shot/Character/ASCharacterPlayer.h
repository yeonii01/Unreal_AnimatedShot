// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/ASCharacterHUDInterface.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "PaperSpriteComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/UserWidget.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ScopeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InventoryAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);
	//void QuaterLook(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	void Attack();
	void Zoom();
	void Inventory();
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ScopeWidgetClass;

	UUserWidget* ScopeWidget;

	bool bIsZoom;
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

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> RespawnWidgetClass;

	UUserWidget* RespawnWidget;

	UPROPERTY()
	class UButton* RespawnButton;

	UFUNCTION()
	void OnRespawnButtonClicked();

	FVector InitialSpawnLocation;
	FRotator InitialSpawnRotation;

private:
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float DefaultFOV;  // ±âº» FOV

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomedFOV = 50.0f;   // ÁÜ ½Ã FOV

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomInterpSpeed = 10.0f;  // ÁÜ ¼Óµµ

	UPROPERTY()
	UCameraComponent* CameraComponent; // Ä«¸Þ¶ó ÂüÁ¶

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSound;
};
