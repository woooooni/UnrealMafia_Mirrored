// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "CustomCirclePanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UCustomCirclePanelWidget : public UCanvasPanel
{
	GENERATED_BODY()

public:
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
protected:
	virtual void ArrangeCircleWidgets();


public:
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
	
};
