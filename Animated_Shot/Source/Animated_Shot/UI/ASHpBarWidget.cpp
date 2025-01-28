// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ASCharacterWidgetInterface.h"
UASHpBarWidget::UASHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.f;
}

void UASHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	IASCharacterWidgetInterface* CharacterWidget = Cast<IASCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetUpCharacterWidget(this);
	}
}

void UASHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
