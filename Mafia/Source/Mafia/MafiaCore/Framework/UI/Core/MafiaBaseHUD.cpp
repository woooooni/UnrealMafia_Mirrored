// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/UI/Core/MafiaBaseHUD.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseUserWidget.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseActionGroupWidget.h"
#include "MafiaCore/Framework/System/MafiaGameEvents.h"
#include "Mafia.h"

#include "Blueprint/WidgetTree.h"

AMafiaBaseHUD::AMafiaBaseHUD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// HUD는 기본적으로 Tick을 끈다
	PrimaryActorTick.bCanEverTick = false;
}

void AMafiaBaseHUD::PostInitializeComponents()
{
	MAFIA_ALOG(LogMafiaWidget, Warning, TEXT("======================= Begin ======================="));
	/** PlayerOwner = Cast<APlayerController>(GetOwner()); */
	Super::PostInitializeComponents();
	MAFIA_ALOG(LogMafiaWidget, Warning, TEXT("End"));
}

void AMafiaBaseHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMafiaBaseHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMafiaBaseHUD::InitializeHUD()
{
	InitializeAllWidget();
}

void AMafiaBaseHUD::InitializeAllWidget()
{
	if (ActionGroupWidgetConfigMap.Num() <= 0)
	{
		bIsCompletedAllLoadWidgets = true;
		return;
	}

	for (const TPair<FName, FMafiaWidgetConfig>& It : ActionGroupWidgetConfigMap)
	{
		const FName& WidgetName = It.Key;
		const FMafiaWidgetConfig& WidgetConfig = It.Value;
		if (WidgetConfig.WidgetClassReference.IsNull())
		{
			ensure(false);
			continue;
		}

		// TSoftClassPtr는 바로 사용할 수 없고, 반드시 LoadSynchronous를 사용해야함.
		UClass* LoadedWidgetClass = WidgetConfig.WidgetClassReference.LoadSynchronous();
		if (false == IsValid(LoadedWidgetClass))
		{
			ensure(false);
			continue;
		}

		APlayerController* PlayerController = GetOwningPlayerController();
		if (false == IsValid(PlayerController))
		{
			ensure(false);
			continue;
		}

		UMafiaBaseUserWidget* NewWidget = CreateWidget<UMafiaBaseUserWidget>(PlayerController, LoadedWidgetClass);
		if (false == IsValid(NewWidget))
		{
			ensure(false);
			continue;
		}

		// 생성한 최상위 위젯들을 별도로 WidgetGroupMap에 추가
		ActionGroupWidgetMap.Emplace(WidgetName, NewWidget);
		NewWidget->SetVisibility(WidgetConfig.bDefaultShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		if (!NewWidget->IsInViewport())
		{
			NewWidget->AddToViewport(WidgetConfig.ZOrder);
		}

		// 진짜 Visible인 녀석들. 예를 들어 버튼?
		if (NewWidget->IsVisible())
		{
			//HandleWidgetsExclusive(WidgetName);
		}

		InitializeAllWidgetMap(WidgetName, NewWidget);
	}

	SendGameEvent(OnInitializedAllWidgets);

	// InitGameEventProcessor();

	bIsCompletedAllLoadWidgets = true;
}

void AMafiaBaseHUD::InitializeAllWidgetMap(const FName& InWidgetName, UMafiaBaseUserWidget* InWidget)
{
	if (false == IsValid(InWidget))
	{
		return;
	}

	UWidgetTree* WidgetTree = InWidget->WidgetTree;
	if (nullptr == WidgetTree)
	{
		return;
	}

	AllWidgetMap.Emplace(InWidgetName, InWidget);
	WidgetTree->ForEachWidget([&](UWidget* InChildWidget)
		{
			UMafiaBaseUserWidget* InMafiaChildWidget = Cast<UMafiaBaseUserWidget>(InChildWidget);
			if (false == IsValid(InMafiaChildWidget))
			{
				return;
			}

			AllWidgetMap.Emplace(InMafiaChildWidget->GetFName(), InMafiaChildWidget);
		});
}

void AMafiaBaseHUD::HandleWidgetsVisibility(const EGameFlowEventsType GameFlowEvent)
{
	const FWidgetVisibleList* FoundedVisibleListPtr = WidgetVisibleConfigMap.Find(GameFlowEvent);
	if (FoundedVisibleListPtr == nullptr)
	{
		return;
	}

	for (TPair<FName, TWeakObjectPtr<UMafiaBaseUserWidget>>& It : ActionGroupWidgetMap)
	{
		const FName& InWidgetName = It.Key;
		UMafiaBaseUserWidget* WidgetGroup = It.Value.Get();
		if (false == IsValid(WidgetGroup))
		{
			continue;
		}

		const FMafiaWidgetConfig* MafiaWidgetConfigPtr = ActionGroupWidgetConfigMap.Find(InWidgetName);
		if (MafiaWidgetConfigPtr == nullptr)
		{
			continue;
		}

		const bool bFoundedWidgetList = FoundedVisibleListPtr->WidgetList.Contains(InWidgetName);
		WidgetGroup->SetVisibility(bFoundedWidgetList ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		if (!WidgetGroup->IsInViewport())
		{
			WidgetGroup->AddToViewport(MafiaWidgetConfigPtr->ZOrder);
		}
	}
}