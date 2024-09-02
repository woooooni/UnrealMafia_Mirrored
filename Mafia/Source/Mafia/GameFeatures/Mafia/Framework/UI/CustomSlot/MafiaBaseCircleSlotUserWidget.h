// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseUserWidget.h"
#include "MafiaBaseCircleSlotUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaBaseCircleSlotUserWidget : public UMafiaBaseUserWidget
{
	GENERATED_BODY()
	
private:
	UMafiaBaseCircleSlotUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;

public:
	virtual bool Initialize() override;

	void UpdateWidget();

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UCanvasPanel> CP_ParentPanel;

private:
	UPROPERTY(EditDefaultsOnly, Category = "CircleSlot")
	uint8 NumWidgets;

	UPROPERTY(EditDefaultsOnly, Category = "CircleSlot")
	FVector InitialRotationAxis;

	UPROPERTY(EditAnywhere, Category = "CircleSlot")
	uint8 bRotateWidgets : 1;

	UPROPERTY(EditAnywhere, Category = "CircleSlot")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "CircleSlot")
	float AngleOfFirstWidget;

	UPROPERTY(EditAnywhere, Category = "CircleSlot")
	float BaseWidgetRotation;
};
