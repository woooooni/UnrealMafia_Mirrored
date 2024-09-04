// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/Viewport/AbilityWidget/MafiaAbilityPlayerCardUserWidget.h"

#include "Components/Button.h"
#include "Mafia/Framework/Player/MafiaPlayerState.h"
#include "Mafia/MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"

UMafiaAbilityPlayerCardUserWidget::UMafiaAbilityPlayerCardUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UMafiaAbilityPlayerCardUserWidget::InitializePlayer(AMafiaBasePlayerState* InPlayerState)
{
	OwnerPlayer = InPlayerState;

	return OwnerPlayer.IsValid();
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
	if (AMafiaBasePlayerState* MyPlayerState = GetOwningPlayerState<AMafiaBasePlayerState>())
	{
		if (OwnerPlayer.IsValid())
		{
			UMafiaBaseRoleComponent* MyPlayerComponent = MyPlayerState->GetRoleComponent();
			if (IsValid(MyPlayerComponent))
			{
				MyPlayerComponent->UseAbility(OwnerPlayer.Get());
			}
		}
	}
}
