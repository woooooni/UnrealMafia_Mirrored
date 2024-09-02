// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/InViewport/MafiaAbilityActionGroupWidget.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/System/MafiaGameEvents.h"
#include "Framework/Components/Role/MafiaBaseRoleComponent.h"


#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

UMafiaAbilityActionGroupWidget::UMafiaAbilityActionGroupWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

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
	
}

void UMafiaAbilityActionGroupWidget::UnBindDelegates()
{
	Super::UnBindDelegates();
	UnbindGameEvent(OnChangedMafiaFlowState, OnChangedMafiaFlowStateHandle);
}

void UMafiaAbilityActionGroupWidget::CreatePlayerCards()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		if (AMafiaBaseGameState* GameState = World->GetGameState<AMafiaBaseGameState>())
		{
			uint32 NumOfPlayer = GameState->GetJoinedUserCount();
			for (auto& Pair : GameState->GetJoinedUserPlayerStateMap())
			{
				if (AMafiaBasePlayerState* PlayerState = Pair.Value.Get())
				{
					if (UMafiaBaseRoleComponent* RoleComponent = PlayerState->GetRoleComponent())
					{
						
					}
				}
			}
		}
	}
}

void UMafiaAbilityActionGroupWidget::OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState)
{
	if (InFlowState == EMafiaFlowState::None)
	{
		CreatePlayerCards();
	}
}


