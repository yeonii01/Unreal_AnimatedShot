// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ASCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "ASComboActionData.h"
#include "Physics/ASCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/ASCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/ASWidgetComponent.h"
#include "UI/ASHpBarWidget.h"
#include "Item/ASWeaponItemData.h"

DEFINE_LOG_CATEGORY(LogASCharacter);

// Sets default values
AASCharacterBase::AASCharacterBase()
{
	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ASCAPSULE);

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MyCharacter/character_01_011.character_01_011'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/MyCharacter/Animations/ABP_AS_Character.ABP_AS_Character_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UASCharacterControlData> ShoulderDataRef(TEXT("/Script/Animated_Shot.ASCharacterControlData'/Game/MyCharacter/CharacterControl/ASC_Shoulder.ASC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UASCharacterControlData> QuaterDataRef(TEXT("/Script/Animated_Shot.ASCharacterControlData'/Game/MyCharacter/CharacterControl/ASC_Quater.ASC_Quater'"));
	if(QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT(" / Script / Engine.AnimMontage'/Game/MyCharacter/Animations/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UASComboActionData> ComboActionDataRef(TEXT("/Script/Animated_Shot.ASComboActionData'/Game/CharacterAction/ASA_ComboAttack.ASA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MyCharacter/Animations/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	//Stat Componenet
	Stat = CreateDefaultSubobject<UASCharacterStatComponent>(TEXT("Stat"));

	//Widget Component
	HpBar = CreateDefaultSubobject<UASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.f, 15.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AASCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AASCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AASCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AASCharacterBase::ReadScroll)));

	//Weapon Component
	Weapon1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon1"));
	Weapon1->SetupAttachment(GetMesh(), TEXT("Bip002_L_HandSocket"));
	Weapon2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon2"));
	Weapon2->SetupAttachment(GetMesh(), TEXT("Bip002_R_HandSocket"));
}

void AASCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AASCharacterBase::SetDead);
}

void AASCharacterBase::SetCharacterControlData(const UASCharacterControlData* CharacterControlData)
{
	//Pawn
	bUseControllerRotationYaw = CharacterControlData -> bUseControllerRotationYaw;
	
	//CharaccterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AASCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AASCharacterBase::ComboActionBegin()
{
	//Combo Status
	CurrentCombo = 1;

	//Movement
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//Animation Setting
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AASCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AASCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AASCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AASCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void AASCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void AASCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetTotalStat().AttackRange * 25;
	const float AttackRadius = 5.f;
	const float AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = (GetActorLocation() - FVector(0.f, 0.f, 30.f)) + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANEL_ASACTION, FCollisionShape::MakeSphere(AttackRadius), Params); //World가 제공하는 함수					
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	
	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
#endif
}

float AASCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);
	return DamageAmount;
}

void AASCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
}

void AASCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(DeadMontage, 1.f);
}

void AASCharacterBase::SetUpCharacterWidget(UASUserWidget* InUserWidget)
{
	UASHpBarWidget* HpBarWidget = Cast<UASHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UASHpBarWidget::UpdateHpBar);
	}
}

void AASCharacterBase::TakeItem(UASItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AASCharacterBase::DrinkPotion(UASItemData* InItemData)
{
	UE_LOG(LogASCharacter, Log, TEXT("Drink Potion"));
}

void AASCharacterBase::EquipWeapon(UASItemData* InItemData)
{
	UASWeaponItemData* WeaponItemData = Cast<UASWeaponItemData>(InItemData);
	
	if (WeaponItemData)
	{
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		Weapon1->SetStaticMesh(WeaponItemData->WeaponMesh.Get());
		Weapon2->SetStaticMesh(WeaponItemData->WeaponMesh.Get());
		if (InItemData->Type == EItemType::Weapon1)
		{
			FASCharacterStat NewModifierStat(0, 100, 0, 0);
			Stat->SetModifierStat(NewModifierStat);
		}
		else
		{
			FASCharacterStat NewModifierStat(0, 50, 20, 0);
			Stat->SetModifierStat(NewModifierStat);
		}
	}
}

void AASCharacterBase::ReadScroll(UASItemData* InItemData)
{
	UE_LOG(LogASCharacter, Log, TEXT("Read Scroll"));
}

int32 AASCharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void AASCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}
