// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/InViewport/MafiaAbilityPlayerCardUserWidget.h"

#include "Components/Button.h"

UMafiaAbilityPlayerCardUserWidget::UMafiaAbilityPlayerCardUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UMafiaAbilityPlayerCardUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMafiaAbilityPlayerCardUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMafiaAbilityPlayerCardUserWidget::BindDelegates()
{
	Super::BindDelegates();
	if (IsValid(BTN_AbilityPlayerCard))
	{
		BTN_AbilityPlayerCard->OnClicked.AddDynamic(this, &UMafiaAbilityPlayerCardUserWidget::OnClickedPlayerAbilityCard);
	}

	// OnChangedMatchStateHandle = BindGameEvent(OnChangedMatchState, &UMafiaActionGroupWidget::OnChangedMatchState);
	// OnChangedMafiaFlowStateHandle = BindGameEvent(OnChangedMafiaFlowState, &UMafiaActionGroupWidget::OnChangedMafiaFlowState);
	
}

void UMafiaAbilityPlayerCardUserWidget::UnBindDelegates()
{
	Super::UnBindDelegates();
	if (IsValid(BTN_AbilityPlayerCard))
	{
		BTN_AbilityPlayerCard->OnClicked.RemoveDynamic(this, &UMafiaAbilityPlayerCardUserWidget::OnClickedPlayerAbilityCard);
	}
}

void UMafiaAbilityPlayerCardUserWidget::OnClickedPlayerAbilityCard()
{

}
