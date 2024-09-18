// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseActionGroupWidget.h"
#include "MafiaChairmanActionGroupWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaChairmanActionGroupWidget : public UMafiaBaseActionGroupWidget
{
	GENERATED_BODY()

private:
	UMafiaChairmanActionGroupWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;

private:
	void OnAffectedAbilityEvent(const class AMafiaBasePlayerState* InOther, const EMafiaRole& InRole);
	void OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UTexture2D> DayImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UTexture2D> NightImage;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UScrollBox> SB_AbilityStatus;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> IMG_Day;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> IMG_Timer;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_SignalText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_DayText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_TimerText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeSignalTextAnimation;

private:
	FDelegateHandle OnAffectedAbilityHandle;
	FDelegateHandle OnChangedMafiaFlowStateHandle;
};
