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
	
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerCardWidgetRef = TEXT("/Game/UI/ActionGruopWidget/Viewport/Ability/WBP_MafiaAbilityPlayerCard.WBP_MafiaAbilityPlayerCard_C");
	if (PlayerCardWidgetRef.Succeeded())
	{
		CardWidgetClass = PlayerCardWidgetRef.Class;
	}
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
	OnClickedPlayerCard = BindGameEvent(OnClickedPlayerCard, &UMafiaAbilityActionGroupWidget::OnClickedCard);
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

void UMafiaAbilityActionGroupWidget::CreatePlayerCards()
{
	UWorld* World = GetWorld();
	
	if (IsValid(World))
	{
		bool bAbilityRole = IsAbilityRole();

		if (AMafiaBaseGameState* GameState = World->GetGameState<AMafiaBaseGameState>())
		{
			uint32 NumOfPlayer = GameState->GetJoinedUserCount();
			int32 Index = 0;
			for (auto& Pair : GameState->GetJoinedUserPlayerStateMap())
			{
				if (AMafiaBasePlayerState* OtherPlayerState = Pair.Value.Get())
				{
					if (OtherPlayerState->GetUniqueId() == GetOwningPlayerState()->GetUniqueId())
					{
						if (UMafiaBaseRoleComponent* MyRoleComponent = OtherPlayerState->GetRoleComponent())
						{
							EMafiaRole MyRole = MyRoleComponent->GetRoleType();
							TB_RoleName->SetText(FText::FromName(MyRoleComponent->GetRoleName()));
							// TB_AbilityToolTip->SetText();
						}
					}
					else
					{
						if (bAbilityRole)
						{
							if (UMafiaBaseRoleComponent* RoleComponent = OtherPlayerState->GetRoleComponent())
							{
								// const FName WidgetName = FName(*FString::Printf(TEXT("PlayerCard %d"), Index));
								UMafiaAbilityPlayerCardUserWidget* CardWidget = CreateWidget<UMafiaAbilityPlayerCardUserWidget>(GetWorld(), CardWidgetClass.Get());
								if (IsValid(CardWidget))
								{
									CardWidget->InitializePlayer(OtherPlayerState);
									CP_PlayerCardCirclePanel->AddChildToCanvas(CardWidget);
									PlayerCards.Add(CardWidget);
								}
							}
						}
					}
				}
				Index++;
			}
		}
	}

	ArrangeCircleCards();
}

void UMafiaAbilityActionGroupWidget::ArrangeCircleCards()
{
	if (IsValid(CP_PlayerCardCirclePanel) == false)
	{
		return;
	}

	CircleWidgetParam.InitialRotationAxis = FVector(0.f, -1.f, 0.f).RotateAngleAxis(CircleWidgetParam.AngleOfFirstWidget, { 0.f, 0.f, 1.f });
	CircleWidgetParam.NumWidgets = CP_PlayerCardCirclePanel->GetSlots().Num();

	uint32 Index = 0;
	const TArray<UWidget*>& Children = CP_PlayerCardCirclePanel->GetAllChildren();

	FAnchors Anchor;
	Anchor.Minimum = { 0.5f, 0.5f };
	Anchor.Maximum = { 0.5f, 0.5f };

	for (auto& ChildSlot : CP_PlayerCardCirclePanel->GetSlots())
	{
		if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(ChildSlot))
		{
			PanelSlot->SetAnchors(Anchor);
			PanelSlot->SetAlignment({ 0.5f, 0.5f });

			float RotateAngleDeg = (360.f / CircleWidgetParam.NumWidgets) * Index;
			FVector RotateVector = CircleWidgetParam.InitialRotationAxis.RotateAngleAxis(RotateAngleDeg, { 0.f, 0.f, 1.f });
			FVector RotatePositionVector = RotateVector * CircleWidgetParam.Radius;

			PanelSlot->SetPosition({ RotatePositionVector.X, RotatePositionVector.Y });
			if (CircleWidgetParam.bRotateWidgets)
			{
				FRotator Rotator = UKismetMathLibrary::MakeRotFromX(RotateVector);
				PanelSlot->Content->SetRenderTransformAngle(CircleWidgetParam.BaseWidgetRotation + Rotator.Yaw);
			}
			else
			{
				PanelSlot->Content->SetRenderTransformAngle(CircleWidgetParam.BaseWidgetRotation);
			}
			Index++;
		}
	}
}

void UMafiaAbilityActionGroupWidget::UpdatePlayerCards()
{
	for(auto Weak : PlayerCards)
	{
		if (Weak.IsValid())
		{
			Weak.Get()->UpdateCard();
		}
	}
}

void UMafiaAbilityActionGroupWidget::ResetCards()
{
	if (IsValid(CP_PlayerCardCirclePanel))
	{
		CP_PlayerCardCirclePanel->ClearChildren();
	}
	PlayerCards.Empty();
}

void UMafiaAbilityActionGroupWidget::ResetForNextRound()
{
	for (auto Weak : PlayerCards)
	{
		if (Weak.IsValid())
		{
			if (UMafiaAbilityPlayerCardUserWidget* CardWidget = Weak.Get())
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
		ResetCards();
		CreatePlayerCards();
	}
}

void UMafiaAbilityActionGroupWidget::OnResponseUseAbility(const UMafiaBaseRoleComponent* InOther, const EMafiaUseAbilityFlag InFlag, const EMafiaAbilityEventType InEventType)
{
	if (InFlag == EMafiaUseAbilityFlag::ImpossibleUseAbility)
	{
		ResetForNextRound();
	}
}

void UMafiaAbilityActionGroupWidget::OnClickedCard()
{
	for (auto Weak : PlayerCards)
	{
		if (Weak.IsValid())
		{
			if (UMafiaAbilityPlayerCardUserWidget* CardWidget = Weak.Get())
			{
				CardWidget->ResetForNextRound();
			}
		}
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


