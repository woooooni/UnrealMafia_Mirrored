// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/World/MafiaPlayerNameActionGroupWidget.h"
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

void UMafiaPlayerNameActionGroupWidget::SetFontSize(const float InFontSize, const float InDeltaTime)
{
	if (FMath::IsWithinInclusive(InFontSize, 5.f, 32.f))
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
		FSlateFontInfo FontInfo = TB_PlayerName->GetFont();
		FontInfo.Size = FMath::Lerp(FontInfo.Size, InFontSize, FMath::Clamp(InDeltaTime * 150.f, 0.f, 1.f));
		TB_PlayerName->SetFont(FontInfo);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMafiaPlayerNameActionGroupWidget::SetTextColor(const FLinearColor& InColor)
{
	if (IsValid(TB_PlayerName))
	{
		TB_PlayerName->SetColorAndOpacity(InColor);
	}
}

