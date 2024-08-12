// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MafiaCore/Framework/Types/MafiaUITypes.h"
#include "MafiaBaseHUD.generated.h"

class UMafiaBaseUserWidget;

/**
 * 모든 UI를 관리하는 HUD
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class MAFIA_API AMafiaBaseHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	AMafiaBaseHUD(const class FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitializeAllWidget();
	void InitializeAllWidgetMap(const FName& InWidgetName, UMafiaBaseUserWidget* InWidget);

public:
	virtual void InitializeHUD();
	void HandleWidgetsVisibility(const EGameFlowEventsType GameFlowEvent);

protected:
	/** 게임에서 사용될 모든 ActionGroupWidget을 Config에 등록 */
	UPROPERTY(EditDefaultsOnly, Category = "WidgetConfig")
	TMap<FName, FMafiaWidgetConfig> ActionGroupWidgetConfigMap;

	/** 게임의 특정 상태에 따라 보여줄 레이어를 결정하는 설정 */ 
	UPROPERTY(EditDefaultsOnly, Category = "Widget Visibility Handle Config")
	TMap<EGameFlowEventsType, FWidgetVisibleList> WidgetVisibleConfigMap;
	
	/** 최상위 WidgetGroup만을 위한 맵 - 최상위 위젯만 반복문 돌리고 싶을때 사용 */
	TMap<FName, TWeakObjectPtr<UMafiaBaseUserWidget>> ActionGroupWidgetMap;

	/** WidgetGroup을 포함하여 그 자식들까지의 모든 위젯이 등록된 맵 - 특정 위젯을 찾고 싶을때 사용 */
	TMap<FName, TWeakObjectPtr<UMafiaBaseUserWidget>> AllWidgetMap;
	bool bIsCompletedAllLoadWidgets = false;
};
