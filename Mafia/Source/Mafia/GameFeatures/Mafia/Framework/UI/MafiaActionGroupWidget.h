// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseActionGroupWidget.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaActionGroupWidget.generated.h"

class UButton;
class UTextBlock;
/**
 *
 */
UCLASS(Abstract)
class MAFIA_API UMafiaActionGroupWidget final : public UMafiaBaseActionGroupWidget
{
	GENERATED_BODY()

private:
	UMafiaActionGroupWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnClickedReadyButton();

	void OnChangedMatchState(const FName& InMatchState);
	void OnChangedMafiaFlowState(const EMafiaFlowState& InMafiaFlowState);

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TB_MatchState;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TB_MafiaFlowState;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TB_UserCount;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TB_Seconds;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> BTN_Ready;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TB_Ready;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TB_Round;
	
private:
	FDelegateHandle OnChangedMatchStateHandle;
	FDelegateHandle OnChangedMafiaFlowStateHandle;
};
