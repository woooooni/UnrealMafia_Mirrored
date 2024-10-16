// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/Viewport/AbilityWidget/MafiaAbilityActionGroupWidget.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/System/MafiaGameEvents.h"
#include "Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

UMafiaAbilityActionGroupWidget::UMafiaAbilityActionGroupWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMafiaAbilityActionGroupWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdatePlayerCards();
}

void UMafiaAbilityActionGroupWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMafiaAbilityActionGroupWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMafiaAbilityActionGroupWidget::BindDelegates()
{
	Super::BindDelegates();
	OnChangedMafiaFlowStateHandle = BindGameEvent(OnChangedMafiaFlowState, &UMafiaAbilityActionGroupWidget::OnChangedMafiaFlowState);
	OnChangedMatchStateHandle = BindGameEvent(OnChangedMatchState, &UMafiaAbilityActionGroupWidget::OnChangedMatchState);
	OnResponseAbilityHandle = BindGameEvent(OnResponseUseAbility, &UMafiaAbilityActionGroupWidget::OnResponseUseAbility);
}

void UMafiaAbilityActionGroupWidget::UnBindDelegates()
{
	Super::UnBindDelegates();
	if (OnChangedMafiaFlowStateHandle.IsValid())
	{
		UnbindGameEvent(OnChangedMafiaFlowState, OnChangedMafiaFlowStateHandle);
	}
	
	if (OnChangedMatchStateHandle.IsValid())
	{
		UnbindGameEvent(OnChangedMatchState, OnChangedMatchStateHandle);
	}
	
}


void UMafiaAbilityActionGroupWidget::InitializeCards()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		ensure(false);
		return;
	}

	AMafiaBaseGameState* GameState = World->GetGameState<AMafiaBaseGameState>();
	if (IsValid(GameState) == false)
	{
		ensure(false);
		return;
	}

	AMafiaBasePlayerState* MyPlayerState = GetOwningPlayerState<AMafiaBasePlayerState>();
	if (IsValid(MyPlayerState) == false)
	{
		ensure(false);
		return;
	}

	UMafiaBaseRoleComponent* MyRoleComponent = MyPlayerState->GetRoleComponent();
	if (IsValid(MyRoleComponent) == false)
	{
		ensure(false);
		return;
	}

	ResetCards();

	if (IsAbilityRole())
	{
		int32 Index = 0;

		TArray<TWeakObjectPtr<AMafiaBasePlayerState>> OutPlayerStateArray;
		GameState->GetJoinedUserPlayerStateMap().GenerateValueArray(OutPlayerStateArray);

		for (UWidget* Widget : CP_AbilityCanvas->GetAllChildren())
		{
			if (UMafiaAbilityPlayerSelectUserWidget* CardWidget = Cast<UMafiaAbilityPlayerSelectUserWidget>(Widget))
			{
				if (OutPlayerStateArray.IsValidIndex(Index))
				{
					if (MyPlayerState->GetUniqueId() != OutPlayerStateArray[Index].Get()->GetUniqueId())
					{
						CardWidget->InitializePlayer(OutPlayerStateArray[Index].Get());
						CardWidget->SetVisibility(ESlateVisibility::Visible);
					}
					Index++;
				}
				else
				{
					CardWidget->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
	
	TB_RoleName->SetText(FText::FromName(MyRoleComponent->GetRoleName()));
	UpdatePlayerCards();
}


void UMafiaAbilityActionGroupWidget::UpdatePlayerCards()
{
	if (IsValid(CP_AbilityCanvas))
	{
		const TArray<UWidget*>& Children = CP_AbilityCanvas->GetAllChildren();
		for (auto& Card : Children)
		{
			if (UMafiaAbilityPlayerSelectUserWidget* CardWidget = Cast<UMafiaAbilityPlayerSelectUserWidget>(Card))
			{
				CardWidget->UpdateInfo();
			}
		}
	}
	
}

void UMafiaAbilityActionGroupWidget::ResetCards()
{
	if (IsValid(CP_AbilityCanvas)) 
	{
		const TArray<UWidget*>& Children = CP_AbilityCanvas->GetAllChildren();
		for (auto& Card : Children)
		{
			if (UMafiaAbilityPlayerSelectUserWidget* CardWidget = Cast<UMafiaAbilityPlayerSelectUserWidget>(Card))
			{
				CardWidget->Reset();
				CardWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UMafiaAbilityActionGroupWidget::ResetForNextRound()
{
	if (IsValid(CP_AbilityCanvas))
	{
		const TArray<UWidget*>& Children = CP_AbilityCanvas->GetAllChildren();
		for (auto& Card : Children)
		{
			if (UMafiaAbilityPlayerSelectUserWidget* CardWidget = Cast<UMafiaAbilityPlayerSelectUserWidget>(Card))
			{
				CardWidget->ResetForNextRound();
			}
		}
	}
}

void UMafiaAbilityActionGroupWidget::OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState)
{
	if (InFlowState != EMafiaFlowState::Night)
	{
		ResetForNextRound();
	}
}

void UMafiaAbilityActionGroupWidget::OnChangedMatchState(const FName& InMatchState)
{
	if (InMatchState == MafiaMatchState::InProgressMafia)
	{
		InitializeCards();
	}
}

void UMafiaAbilityActionGroupWidget::OnResponseUseAbility(const AMafiaBasePlayerState* InOther, const EMafiaUseAbilityFlag InFlag, const EMafiaAbilityEventType InEventType)
{
	if (InFlag == EMafiaUseAbilityFlag::ImpossibleUseAbility)
	{
		ResetForNextRound();
	}
}


bool UMafiaAbilityActionGroupWidget::IsAbilityRole()
{
	if (AMafiaBasePlayerState* MyPlayerState = GetOwningPlayerState<AMafiaBasePlayerState>())
	{
		if (UMafiaBaseRoleComponent* MyRoleComponent = MyPlayerState->GetRoleComponent())
		{
			EMafiaRole MyRole = MyRoleComponent->GetRoleType();
			return !(MyRole == EMafiaRole::Citizen || MyRole == EMafiaRole::Soldier || MyRole == EMafiaRole::Thief);
		}
	}
	return false;
}


