// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASPartyCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"

AASPartyCharacterPlayer::AASPartyCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MyCharacter/character_02_01.character_02_01'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageMontageRef(TEXT("/Game/MyCharacter/Animations/AM_Damage.AM_Damage"));
	if (DamageMontageRef.Object)
	{
		DamageMontage = DamageMontageRef.Object;
	}
}

void AASPartyCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox = Cast<AAASItemWeaponBox>(UGameplayStatics::GetActorOfClass(GetWorld(), AAASItemWeaponBox::StaticClass()));
}

void AASPartyCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AASPartyCharacterPlayer::PlayDamageMontage()
{
	if (DamageMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DamageMontage);
		}
	}
}

void AASPartyCharacterPlayer::EquipWeapon(bool index)
{
	WeaponBox->OpenBox();
	WeaponBox->SelectWeapon(index, this);
}