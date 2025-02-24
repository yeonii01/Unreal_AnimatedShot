// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ASCharacterControlData.h"
#include "UI/ASHUDWidget.h"
#include "PaperSprite.h"
#include "CharacterStat/ASCharacterStatComponent.h"

AASCharacterPlayer::AASCharacterPlayer()
{
	//Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MyCharacter/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MyCharacter/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MyCharacter/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MyCharacter/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MyCharacter/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MyCharacter/Input/Actions/IA_QuaterLook.IA_QuaterLook'"));
	//if (nullptr != InputActionQuaterLookRef.Object)
	//{
	//	QuaterLookAction = InputActionQuaterLookRef.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MyCharacter/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageMontageRef(TEXT("/Game/MyCharacter/Animations/AM_Damage.AM_Damage"));
	if (DamageMontageRef.Object)
	{
		DamageMontage = DamageMontageRef.Object;
	}

	/** 미니맵용 스프링암 생성 */
	MinimapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArm"));
	MinimapSpringArm->SetupAttachment(RootComponent);
	MinimapSpringArm->TargetArmLength = 2000.0f; // 위쪽으로 멀리 떨어진 거리
	MinimapSpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f)); // 아래를 바라보도록 회전
	MinimapSpringArm->bDoCollisionTest = false;
	MinimapSpringArm->bEnableCameraLag = false;
	MinimapSpringArm->bEnableCameraRotationLag = false;
	MinimapSpringArm->bUsePawnControlRotation = false; // 캐릭터의 회전값을 따라가지 않도록 설정
	MinimapSpringArm->bInheritPitch = false;
	MinimapSpringArm->bInheritYaw = false;
	MinimapSpringArm->bInheritRoll = false;


	/** Scene Capture Component 생성 */
	MinimapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCapture"));
	MinimapCapture->SetupAttachment(MinimapSpringArm);
	MinimapCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	MinimapCapture->OrthoWidth = 3000.0f; // 미니맵의 범위
	MinimapCapture->bCaptureEveryFrame = true;
	MinimapCapture->bCaptureOnMovement = true;

	static ConstructorHelpers::FObjectFinder<UCanvasRenderTarget2D> MinimapRenderTargetRef(TEXT("/Game/UI/Minimap_Target.Minimap_Target"));
	if (MinimapRenderTargetRef.Succeeded())
	{
		MinimapCanvasRenderTarget = MinimapRenderTargetRef.Object;
	}

	MinimapIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapIcon"));
	MinimapIcon->SetupAttachment(RootComponent);

	/** 아이콘용 스프라이트 로드 */
	static ConstructorHelpers::FObjectFinder<UPaperSprite> PlayerIconSpriteRef(TEXT("/Game/UI/PlayerIcon_Sprite.PlayerIcon_Sprite"));
	if (PlayerIconSpriteRef.Succeeded())
	{
		MinimapIcon->SetSprite(PlayerIconSpriteRef.Object); 
	}

	/** 아이콘 크기 조정 */
	MinimapIcon->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	MinimapIcon->SetRelativeRotation(FRotator(0.f, -90.f, 90.f));
	MinimapIcon->SetRelativeLocation(FVector(0.0f, 0.0f, 900.0f));
}

void AASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) EnableInput(PlayerController);

	SetCharacterControl(CurrentCharacterControlType);

	if (MinimapCanvasRenderTarget)
	{
		MinimapCapture->TextureTarget = MinimapCanvasRenderTarget;
	}
}

void AASCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void AASCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked< UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::Attack);
	//EnhancedInputComponent->BindAction(QuaterLookAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::QuaterLook);
}

void AASCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AASCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UASCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AASCharacterPlayer::SetCharacterControlData(const UASCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeLocation(CharacterControlData->RelativeRotation.Vector());
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AASCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AASCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AASCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AASCharacterPlayer::Attack()
{
	ProcessComboCommand();
}

float AASCharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DamageMontage);
		}
	}

	return AASCharacterBase::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AASCharacterPlayer::SetupHUDWidget(UASHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UASHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UASHUDWidget::UpdateHpBar);
	}
}

//void AASCharacterPlayer::QuaterLook(const FInputActionValue& Value)
//{
//}
