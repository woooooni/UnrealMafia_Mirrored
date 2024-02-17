// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/MafiaActionGroupWidget.h"
#include "GameFeatures/Mafia/Framework/GameModes/MafiaGameState.h"
#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerState.h"
#include "MafiaCore/Framework/System/MafiaGameEvents.h"
#include "GameFramework/GameMode.h"
#include "Mafia.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

UMafiaActionGroupWidget::UMafiaActionGroupWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMafiaActionGroupWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMafiaActionGroupWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMafiaActionGroupWidget::BindDelegates()
{
	Super::BindDelegates();

	if (IsValid(BTN_Ready))
	{
		BTN_Ready->OnClicked.AddDynamic(this, &UMafiaActionGroupWidget::OnClickedReadyButton);
	}

	OnChangedMatchStateHandle = BindGameEvent(OnChangedMatchState, &UMafiaActionGroupWidget::OnChangedMatchState);
	OnChangedMafiaFlowStateHandle = BindGameEvent(OnChangedMafiaFlowState, &UMafiaActionGroupWidget::OnChangedMafiaFlowState);
}

void UMafiaActionGroupWidget::UnBindDelegates()
{
	Super::UnBindDelegates();

	if (IsValid(BTN_Ready))
	{
		BTN_Ready->OnClicked.RemoveDynamic(this, &UMafiaActionGroupWidget::OnClickedReadyButton);
	}

	UnbindGameEvent(OnChangedMatchState, OnChangedMatchStateHandle);
	UnbindGameEvent(OnChangedMafiaFlowState, OnChangedMafiaFlowStateHandle);
}

void UMafiaActionGroupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// #Todo-ksh : 매 틱마다 시간 찍는것은 비효율적이라 개선 필요. 일단은 간단하게 이렇게 작업해둠
	//				나중에 ui로 작업될 수 있는 부분이라, 거기서 수정하는게 맞아보임
	AMafiaBaseGameState* GameState = GetWorld()->GetGameState<AMafiaBaseGameState>();
	if (false == IsValid(GameState))
	{
		return;
	}

	const float GameFlowTime = GameState->GetGameFlowTimerRemaining();
	if (FMath::IsNearlyZero(GameFlowTime))
	{
		// 눈에 보이기 쉽게 일단 return하지 않음 - Sunghoon
		//return;
	}

	if (IsValid(TB_UserCount))
	{
		FString UserCountText;
		UserCountText.Reserve(33);
		if (GameState->GetMatchState() == MatchState::WaitingToStart)
		{
			UserCountText = FString::Printf(TEXT("ReadyCount:%d, "), GameState->GetReadyUserCount());
		}
		
		UserCountText.Append(FString::Printf(TEXT("UserCount:%d"), GameState->GetJoinedUserCount()));
		TB_UserCount->SetText(FText::FromString(UserCountText));
	}

	if (IsValid(TB_Seconds))
	{
		const int32 GameFlowTimeInt = (GameFlowTime + 0.5f);
		FString FlowTimeText = TEXT("Time : ");
		FlowTimeText.Append(FString::FromInt(GameFlowTimeInt));
		TB_Seconds->SetText(FText::FromString(FlowTimeText));
	}
}

void UMafiaActionGroupWidget::OnClickedReadyButton()
{
	MAFIA_MESSAGE(OnClickedReadyButton);

	if (false == IsValid(TB_Ready))
	{
		ensure(false);
		return;
	}

	AMafiaPlayerState* MyPlayerState = GetOwningPlayerState<AMafiaPlayerState>();
	if (false == IsValid(MyPlayerState))
	{
		ensure(false);
		return;
	}

	if (BTN_Ready->GetBackgroundColor() == FLinearColor::White)
	{
		BTN_Ready->SetBackgroundColor(FLinearColor::Black);
		TB_Ready->SetText(FText::FromString(TEXT("Cancel")));
		MyPlayerState->ServerReqReady(true);
	}
	else if (BTN_Ready->GetBackgroundColor() == FLinearColor::Black)
	{
		BTN_Ready->SetBackgroundColor(FLinearColor::White);
		TB_Ready->SetText(FText::FromString(TEXT("Ready")));
		MyPlayerState->ServerReqReady(false);
	}
}

void UMafiaActionGroupWidget::OnChangedMatchState(const FName& InMatchState)
{
	if (false == IsValid(TB_MatchState))
	{
		ensure(false);
		return;
	}

	FString MatchStateText = TEXT("MatchState : ");
	MatchStateText.Append(InMatchState.ToString());
	TB_MatchState->SetText(FText::FromString(MatchStateText));

	if (InMatchState == MatchState::InProgress)
	{
		if (IsValid(BTN_Ready))
		{
			BTN_Ready->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMafiaActionGroupWidget::OnChangedMafiaFlowState(const EMafiaFlowState& InMafiaFlowState)
{
	if (false == IsValid(TB_MafiaFlowState))
	{
		ensure(false);
		return;
	}

	FString MafiaFlowStateText = TEXT("MafiaFlowState : ");
	MafiaFlowStateText.Append(EnumToStringMafia(EMafiaFlowState, InMafiaFlowState));
	TB_MafiaFlowState->SetText(FText::FromString(MafiaFlowStateText));
}
