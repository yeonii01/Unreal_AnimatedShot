// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASHUDWidget.h"
#include "Interface/ASCharacterHUDInterface.h"
#include "ASHpBarWidget.h"
#include "ASCharacterStatWidget.h"


UASHUDWidget::UASHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UASHUDWidget::UpdateStat(const FASCharacterStat& BaseStat, const FASCharacterStat& ModifierStat)
{
	FASCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHp);

	CharacterStat->UpdateStat(BaseStat, ModifierStat);

}

void UASHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UASHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UASHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UASCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	IASCharacterHUDInterface* HUDPawn = Cast<IASCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
