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

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;

private:
	virtual void OnStartAnimation() override;
	void TickAnimation();
	virtual void OnEndAnimation() override;

private:
	void CreatePlayerCards();
	void ArrangeCircleCards();
	void UpdatePlayerCards();
	void ResetCards();

private:
	void ResetForNextRound();

private:
	void OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState);
	void OnChangedMatchState(const FName& InMatchState);
	void OnResponseUseAbility(const class AMafiaBasePlayerState* InOther, const EMafiaUseAbilityFlag InFlag, const EMafiaAbilityEventType InEventType);
	void OnClickedCard();



private:
	bool IsAbilityRole();


	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UMG")
	uint8 NumWidgets;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UMG")
	FVector InitialRotationAxis;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "UMG")
	float Radius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UMG")
	float AngleOfFirstWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UMG")
	float BaseWidgetRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UMG")
	uint8 bRotateWidgets : 1;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UCanvasPanel> CP_PlayerCardCirclePanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> IMG_AbilityIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_RoleName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_AbilityToolTip;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	TObjectPtr<class UWidgetAnimation> ArrangeCircleCardAnimation;

private:
	TArray<TWeakObjectPtr<class UMafiaAbilityPlayerCardUserWidget>> PlayerCards;

private:
	TSubclassOf<class UMafiaAbilityPlayerCardUserWidget> CardWidgetClass;

private:
	FDelegateHandle OnChangedMatchStateHandle;
	FDelegateHandle OnChangedMafiaFlowStateHandle;
	FDelegateHandle OnClickedPlayerCard;
	FDelegateHandle OnResponseAbilityHandle;
	
	FTimerHandle OnAnimationTickHandle;
	
};
