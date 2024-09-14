// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseActionGroupWidget.h"
#include "GameFeatures/Mafia/Framework/UI/Viewport/AbilityWidget/MafiaAbilityPlayerCardUserWidget.h"
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

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;

private:
	void InitializeCards();
	void ArrangeCircleCards();
	void UpdatePlayerCards();
	void ResetCards();

private:
	void ResetForNextRound();

private:
	void OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState);
	void OnChangedMatchState(const FName& InMatchState);
	void OnResponseUseAbility(const class AMafiaBasePlayerState* InOther, const EMafiaUseAbilityFlag InFlag, const EMafiaAbilityEventType InEventType);

private:
	bool IsAbilityRole();


	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Card Class")
	TSoftClassPtr<class UMafiaBaseUserWidget> CardWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Circle Arrange")
	uint8 NumWidgets;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Circle Arrange")
	FVector InitialRotationAxis;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Circle Arrange")
	float Radius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Circle Arrange")
	float AngleOfFirstWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Circle Arrange")
	float BaseWidgetRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Circle Arrange")
	uint8 bRotateWidgets : 1;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UCanvasPanel> CP_AbilityCanvas;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> IMG_AbilityIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_RoleName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_AbilityToolTip;

private:
	FDelegateHandle OnChangedMatchStateHandle;
	FDelegateHandle OnChangedMafiaFlowStateHandle;
	FDelegateHandle OnClickedPlayerCard;
	FDelegateHandle OnResponseAbilityHandle;	
	
};
