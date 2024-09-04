// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseActionGroupWidget.h"
#include "GameFeatures/Mafia/Framework/UI/Viewport/AbilityWidget/MafiaAbilityPlayerCardUserWidget.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaAbilityActionGroupWidget.generated.h"

USTRUCT(BlueprintType)
struct FCircleWidgetParam
{
	GENERATED_BODY();

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 NumWidgets;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector InitialRotationAxis;

	UPROPERTY(BlueprintReadWrite)
	float Radius;

	UPROPERTY(BlueprintReadWrite)
	float AngleOfFirstWidget;

	UPROPERTY(BlueprintReadWrite)
	float BaseWidgetRotation;

	UPROPERTY(BlueprintReadWrite)
	uint8 bRotateWidgets : 1;
};

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaAbilityActionGroupWidget : public UMafiaBaseActionGroupWidget
{
	GENERATED_BODY()

private:
	UMafiaAbilityActionGroupWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;

private:
	void CreatePlayerCards();

private:
	void OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState);
	
public:
	UPROPERTY(BlueprintReadWrite)
	FCircleWidgetParam CircleWidgetParam;

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
