// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/Viewport/ChairMan/MafiaChairmanActionGroupWidget.h"
#include "GameFeatures/Mafia/Framework/UI/Viewport/ChairMan/MafiaChairmanLogScrollUserWidget.h"

#include "Mafia/Framework/GameModes/MafiaGameState.h"
#include "MafiaCore/Framework/System/MafiaGameEvents.h"


#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Animation/WidgetAnimation.h"



UMafiaChairmanActionGroupWidget::UMafiaChairmanActionGroupWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UMafiaChairmanActionGroupWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMafiaChairmanActionGroupWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMafiaChairmanActionGroupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		return;
	}

	AMafiaBaseGameState* GameState = World->GetGameState<AMafiaBaseGameState>();
	if (IsValid(GameState) == false)
	{
		return;
	}

	if (IsValid(TB_TimerText) == false)
	{
		return;
	}

	uint32 RemainTime = GameState->GetGameFlowTimerRemaining();
	FString RemainTimeStr = FString::Printf(TEXT("%d초"), RemainTime);
	TB_TimerText->SetText(FText::FromString(RemainTimeStr));
}

void UMafiaChairmanActionGroupWidget::BindDelegates()
{
	Super::BindDelegates();
	OnAffectedAbilityHandle = BindGameEvent(OnAffectedAbilityEvent, &UMafiaChairmanActionGroupWidget::OnAffectedAbilityEvent);
	OnChangedMafiaFlowStateHandle = BindGameEvent(OnChangedMafiaFlowState, &UMafiaChairmanActionGroupWidget::OnChangedMafiaFlowState);
	
}

void UMafiaChairmanActionGroupWidget::UnBindDelegates()
{
	Super::UnBindDelegates();
	if (OnAffectedAbilityHandle.IsValid())
	{
		UnbindGameEvent(OnAffectedAbilityEvent, OnAffectedAbilityHandle);
	}

	if (OnChangedMafiaFlowStateHandle.IsValid())
	{
		UnbindGameEvent(OnChangedMafiaFlowState, OnChangedMafiaFlowStateHandle);
	}
}

void UMafiaChairmanActionGroupWidget::Veil()
{
	PlayAnimation(VeilFadeAnimation);
}

void UMafiaChairmanActionGroupWidget::UnVeil()
{
	PlayAnimationReverse(VeilFadeAnimation);
}

void UMafiaChairmanActionGroupWidget::OnAffectedAbilityEvent(const AMafiaBasePlayerState* InOther, const EMafiaRole& InRole)
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		return;
	}

	AMafiaBaseGameState* GameState = World->GetGameState<AMafiaBaseGameState>();
	if (IsValid(GameState) == false)
	{
		return;
	}

	FString LogTextStr = FString::Printf(TEXT("%d일차 : "), GameState->GetGameRound());
	switch (InRole)
	{
	case EMafiaRole::Doctor:
		LogTextStr += TEXT("의사가 혼신의 힘을 다해 당신을 살렸습니다.");
		PlayAnimation(HealVeilAnimation);
		break;
	case EMafiaRole::Killer:
		LogTextStr += TEXT("연쇄살인마의 칼에 무참히 살해됐습니다.");
		PlayAnimation(MurederedVeilAnimation);
		break;
	case EMafiaRole::GodFather:
		LogTextStr += TEXT("마피아의 총에 맞아 사망했습니다.");
		PlayAnimation(MurederedVeilAnimation);
		break;
	case EMafiaRole::Madam:
		LogTextStr += TEXT("마담의 유혹에 당해 이번 밤의 능력이 무효화됩니다.");
		PlayAnimation(MadamVeilAnimation);
		break;
	case EMafiaRole::BusDriver:
		LogTextStr += TEXT("버스의 승객이 되어 여행을 떠납니다.");
		break;
	}


	/*for (auto& Child : SB_AbilityStatus->GetAllChildren())
	{
		if (Child->GetVisibility() == ESlateVisibility::Collapsed)
		{
			if (UTextBlock* ChildTextBlock = Cast<UTextBlock>(Child))
			{
				ChildTextBlock->SetText(FText::FromString(LogTextStr));
				ChildTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				return;
			}
		}
	}*/
}

void UMafiaChairmanActionGroupWidget::OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState)
{
	if (IsValid(TB_DayText) == false)
	{
		return;
	}
	if (IsValid(IMG_Day) == false)
	{
		return;
	}

	if (InFlowState == EMafiaFlowState::Day)
	{
		IMG_Day->SetBrushFromSoftTexture(DayImage);
		TB_DayText->SetText(FText::FromName(TEXT("아침")));
		TB_SignalText->SetText(FText::FromName(TEXT("아침이 되었습니다.")));

		PlayAnimation(FadeSignalTextAnimation);
		UnVeil();
	}
	else if (InFlowState == EMafiaFlowState::BeforeVote)
	{
		IMG_Day->SetBrushFromSoftTexture(VoteImage);
		TB_DayText->SetText(FText::FromName(TEXT("투표")));
		TB_SignalText->SetText(FText::FromName(TEXT("투표시간입니다.")));

		PlayAnimation(FadeSignalTextAnimation);
	}
	else if (InFlowState == EMafiaFlowState::BeforeNight)
	{
		IMG_Day->SetBrushFromSoftTexture(NightImage);
		TB_DayText->SetText(FText::FromName(TEXT("밤")));
		TB_SignalText->SetText(FText::FromName(TEXT("밤이 되었습니다.")));

		PlayAnimation(FadeSignalTextAnimation);
	}
	else if (InFlowState == EMafiaFlowState::AfterNight)
	{
		Veil();
	}
}

