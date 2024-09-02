// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseActionGroupWidget.h"
#include "GameFeatures/Mafia/Framework/UI/InViewport/MafiaAbilityPlayerCardUserWidget.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaAbilityActionGroupWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaAbilityActionGroupWidget : public UMafiaBaseActionGroupWidget
{
	GENERATED_BODY()

private:
	UMafiaAbilityActionGroupWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;

private:
	void CreatePlayerCards();

private:
	void OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState);
	
private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UCanvasPanel> CP_RoundPlayerPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> IMG_AbilityIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_AbilityToolTip;

private:
	FDelegateHandle OnChangedMafiaFlowStateHandle;
};
