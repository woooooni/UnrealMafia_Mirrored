// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/Viewport/AbilityWidget/MafiaAbilityPlayerSelectUserWidget.h"

#include "Mafia/Framework/Player/MafiaPlayerState.h"
#include "Mafia/MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Framework/System/MafiaGameEvents.h"

UMafiaAbilityPlayerSelectUserWidget::UMafiaAbilityPlayerSelectUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}



void UMafiaAbilityPlayerSelectUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMafiaAbilityPlayerSelectUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMafiaAbilityPlayerSelectUserWidget::BindDelegates()
{
	Super::BindDelegates();
	if (IsValid(BTN_AbilityPlayerCard))
	{
		BTN_AbilityPlayerCard->OnClicked.AddDynamic(this, &UMafiaAbilityPlayerSelectUserWidget::OnClickedPlayerAbilityCard);
	}

	// OnChangedMatchStateHandle = BindGameEvent(OnChangedMatchState, &UMafiaActionGroupWidget::OnChangedMatchState);
	// OnChangedMafiaFlowStateHandle = BindGameEvent(OnChangedMafiaFlowState, &UMafiaActionGroupWidget::OnChangedMafiaFlowState);
}

void UMafiaAbilityPlayerSelectUserWidget::UnBindDelegates()
{
	Super::UnBindDelegates();
	if (IsValid(BTN_AbilityPlayerCard))
	{
		BTN_AbilityPlayerCard->OnClicked.RemoveDynamic(this, &UMafiaAbilityPlayerSelectUserWidget::OnClickedPlayerAbilityCard);
	}
}


bool UMafiaAbilityPlayerSelectUserWidget::InitializePlayer(AMafiaBasePlayerState* InPlayerState)
{
	if (IsValid(InPlayerState))
	{
		OwnerPlayer = InPlayerState;
		UpdateInfo();
	}


	return OwnerPlayer.IsValid();
}

void UMafiaAbilityPlayerSelectUserWidget::UpdateInfo()
{
	if (OwnerPlayer.IsValid())
	{
		if (UMafiaBaseRoleComponent* RoleComponent = OwnerPlayer.Get()->GetRoleComponent())
		{
			EMafiaColor PlayerColor = OwnerPlayer.Get()->GetPlayerColor();
			int32 Index = int32(PlayerColor);

			if (GPlayerColorKoreanNames.IsValidIndex(Index))
			{
				IMG_PlayerColor->SetBrushTintColor(FSlateColor(GPlayerColors[Index]));
				IMG_AbilityIcon->SetBrushTintColor(FSlateColor(GPlayerColors[Index]));
				TB_PlayerColor->SetText(FText::FromName(GPlayerColorKoreanNames[Index]));
			}
		}
	}
	else
	{
		IMG_PlayerColor->SetBrushTintColor(FSlateColor(FLinearColor(1, 1, 1, 1)));
		IMG_AbilityIcon->SetBrushTintColor(FSlateColor(FLinearColor(1, 1, 1, 1)));
		TB_PlayerColor->SetText(FText::FromName(TEXT("Player None")));
	}

}

void UMafiaAbilityPlayerSelectUserWidget::ResetForNextRound()
{
	BTN_AbilityPlayerCard->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.1f));	
}

void UMafiaAbilityPlayerSelectUserWidget::Reset()
{
	OwnerPlayer = nullptr;
	UpdateInfo();
}


bool UMafiaAbilityPlayerSelectUserWidget::IsOwnerPlayer(AMafiaBasePlayerState* InPlayerState)
{
	if (IsValid(InPlayerState) && OwnerPlayer.IsValid())
	{
		return InPlayerState->GetUniqueId() == OwnerPlayer.Get()->GetUniqueId();
	}
	return false;
}

void UMafiaAbilityPlayerSelectUserWidget::OnClickedPlayerAbilityCard()
{
	SendGameEvent(OnClickedPlayerCard);

	if (AMafiaBasePlayerState* MyPlayerState = GetOwningPlayerState<AMafiaBasePlayerState>())
	{
		if (OwnerPlayer.IsValid())
		{
			UMafiaBaseRoleComponent* MyPlayerComponent = MyPlayerState->GetRoleComponent();
			if (IsValid(MyPlayerComponent))
			{
				MyPlayerComponent->UseAbility(OwnerPlayer.Get());
				BTN_AbilityPlayerCard->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			}
		}
	}
}
