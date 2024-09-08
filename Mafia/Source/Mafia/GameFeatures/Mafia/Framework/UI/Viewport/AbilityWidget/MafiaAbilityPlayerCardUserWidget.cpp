// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/Viewport/AbilityWidget/MafiaAbilityPlayerCardUserWidget.h"


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


bool UMafiaAbilityPlayerCardUserWidget::InitializePlayer(AMafiaBasePlayerState* InPlayerState)
{
	if (IsValid(InPlayerState))
	{
		OwnerPlayer = InPlayerState;
		UpdateCard();
	}


	return OwnerPlayer.IsValid();
}

void UMafiaAbilityPlayerCardUserWidget::UpdateCard()
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

}

void UMafiaAbilityPlayerCardUserWidget::ResetForNextRound()
{
	BTN_AbilityPlayerCard->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.1f));
}


bool UMafiaAbilityPlayerCardUserWidget::IsOwnerPlayer(AMafiaBasePlayerState* InPlayerState)
{
	if (IsValid(InPlayerState) && OwnerPlayer.IsValid())
	{
		return InPlayerState->GetUniqueId() == OwnerPlayer.Get()->GetUniqueId();
	}
	return false;
}

void UMafiaAbilityPlayerCardUserWidget::OnClickedPlayerAbilityCard()
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

				BTN_AbilityPlayerCard->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f ));
			}
		}
	}
}
