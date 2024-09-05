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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Circle Param")
	uint8 NumWidgets;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Circle Param")
	FVector InitialRotationAxis;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Circle Param")
	float Radius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Circle Param")
	float AngleOfFirstWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Circle Param")
	float BaseWidgetRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Circle Param")
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
	void ArrangeCircleCards();
	void UpdatePlayerCards();
	void ResetCards();

private:
	void OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState);
	void OnChangedMatchState(const FName& InMatchState);

private:
	bool IsAbilityRole();
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Circle Param")
	FCircleWidgetParam CircleWidgetParam;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UCanvasPanel> CP_PlayerCardCirclePanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> IMG_AbilityIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_RoleName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_AbilityToolTip;

private:
	TArray<TWeakObjectPtr<class UMafiaAbilityPlayerCardUserWidget>> PlayerCards;

private:
	TSubclassOf<class UMafiaAbilityPlayerCardUserWidget> CardWidgetClass;

private:
	FDelegateHandle OnChangedMatchStateHandle;
	FDelegateHandle OnChangedMafiaFlowStateHandle;
	
};
