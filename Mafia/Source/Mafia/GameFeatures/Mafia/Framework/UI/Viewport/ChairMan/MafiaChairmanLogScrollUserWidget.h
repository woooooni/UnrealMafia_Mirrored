// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseUserWidget.h"
#include "MafiaChairmanLogScrollUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaChairmanLogScrollUserWidget : public UMafiaBaseUserWidget
{
	GENERATED_BODY()
private:
	UMafiaChairmanLogScrollUserWidget(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	void AddLogText(const FString& InLogStr);



private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UScrollBox> SB_Log;

private:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> MouseOnAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> MouseLeaveAnimation;

	uint32 VisibleTextIndex;
};
