// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/InGame/MafiaPlayerNameActionGroupWidget.h"
#include "Framework/System/MafiaGameEvents.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Framework/GameModes/MafiaBaseGameState.h"
#include "Mafia/Framework/Character/MafiaSampleCharacter.h"

UMafiaPlayerNameActionGroupWidget::UMafiaPlayerNameActionGroupWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UMafiaPlayerNameActionGroupWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMafiaPlayerNameActionGroupWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMafiaPlayerNameActionGroupWidget::BindDelegates()
{
	Super::BindDelegates();
}

void UMafiaPlayerNameActionGroupWidget::UnBindDelegates()
{
	Super::UnBindDelegates();
}

void UMafiaPlayerNameActionGroupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMafiaPlayerNameActionGroupWidget::SetPlayerName(const FName& InNewPlayerName)
{
	if (IsValid(TB_PlayerName))
	{
		TB_PlayerName->SetText(FText::FromName(InNewPlayerName));
	}
}

void UMafiaPlayerNameActionGroupWidget::SetTextColor(const FLinearColor& InColor)
{
	if (IsValid(TB_PlayerName))
	{
		TB_PlayerName->SetColorAndOpacity(InColor);
	}
}

