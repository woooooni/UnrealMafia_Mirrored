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
	ArrangeCircleCards();
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
			if (UMafiaAbilityPlayerCardUserWidget* CardWidget = Cast<UMafiaAbilityPlayerCardUserWidget>(Widget))
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

	ArrangeCircleCards();
	UpdatePlayerCards();
}

void UMafiaAbilityActionGroupWidget::ArrangeCircleCards()
{
	if (IsValid(CP_AbilityCanvas) == false)
	{
		return;
	}

	NumWidgets = 0;
	for (auto& Widget : CP_AbilityCanvas->GetAllChildren())
	{
		if (UMafiaAbilityPlayerCardUserWidget* CardWidget = Cast<UMafiaAbilityPlayerCardUserWidget>(Widget))
		{
			if (CardWidget->GetVisibility() != ESlateVisibility::Collapsed)
				NumWidgets++;
		}
	}

	InitialRotationAxis = FVector(0.f, -1.f, 0.f).RotateAngleAxis(AngleOfFirstWidget, { 0.f, 0.f, 1.f });
	if (0 < NumWidgets)
	{
		uint32 Index = 0;

		FAnchors Anchor;
		Anchor.Minimum = { 0.5f, 0.5f };
		Anchor.Maximum = { 0.5f, 0.5f };

		for (UPanelSlot* PanelSlot : CP_AbilityCanvas->GetSlots())
		{
			if (UMafiaAbilityPlayerCardUserWidget* CardWidget = Cast<UMafiaAbilityPlayerCardUserWidget>(PanelSlot->Content))
			{
				if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
				{
					CanvasSlot->SetAnchors(Anchor);
					CanvasSlot->SetAlignment({ 0.5f, 0.5f });

					float RotateAngleDeg = (360.f / NumWidgets) * Index;
					FVector RotateVector = InitialRotationAxis.RotateAngleAxis(RotateAngleDeg, { 0.f, 0.f, 1.f });
					FVector RotatePositionVector = RotateVector * Radius;

					CanvasSlot->SetPosition({ RotatePositionVector.X, RotatePositionVector.Y });
					if (bRotateWidgets)
					{
						FRotator Rotator = UKismetMathLibrary::MakeRotFromX(RotateVector);
						CardWidget->SetRenderTransformAngle(BaseWidgetRotation + Rotator.Yaw);
					}
					else
					{
						CardWidget->SetRenderTransformAngle(BaseWidgetRotation);
					}
					Index++;
				}
				
			}
		}
	}

	
}

void UMafiaAbilityActionGroupWidget::UpdatePlayerCards()
{
	const TArray<UWidget*>& Children = CP_AbilityCanvas->GetAllChildren();
	for (auto& Card : Children)
	{
		if (UMafiaAbilityPlayerCardUserWidget* CardWidget = Cast<UMafiaAbilityPlayerCardUserWidget>(Card))
		{
			CardWidget->UpdateCard();
		}
	}
}

void UMafiaAbilityActionGroupWidget::ResetCards()
{
	const TArray<UWidget*>& Children = CP_AbilityCanvas->GetAllChildren();
	for (auto& Card : Children)
	{
		if (UMafiaAbilityPlayerCardUserWidget* CardWidget = Cast<UMafiaAbilityPlayerCardUserWidget>(Card))
		{
			CardWidget->Reset();
			CardWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMafiaAbilityActionGroupWidget::ResetForNextRound()
{
	const TArray<UWidget*>& Children = CP_AbilityCanvas->GetAllChildren();
	for (auto& Card : Children)
	{
		if (UMafiaAbilityPlayerCardUserWidget* CardWidget = Cast<UMafiaAbilityPlayerCardUserWidget>(Card))
		{
			CardWidget->ResetForNextRound();
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


