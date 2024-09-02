// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/CustomSlot/MafiaBaseCircleSlotUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"

UMafiaBaseCircleSlotUserWidget::UMafiaBaseCircleSlotUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AngleOfFirstWidget = 0.f;
}

void UMafiaBaseCircleSlotUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateWidget();
}

void UMafiaBaseCircleSlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMafiaBaseCircleSlotUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMafiaBaseCircleSlotUserWidget::BindDelegates()
{
	Super::BindDelegates();
}

void UMafiaBaseCircleSlotUserWidget::UnBindDelegates()
{
	Super::UnBindDelegates();
}

bool UMafiaBaseCircleSlotUserWidget::Initialize()
{
	if (Super::Initialize() == false)
	{
		return false;
	}

	if (IsValid(CP_ParentPanel))
	{
		UpdateWidget();
	}

	return true;
}

void UMafiaBaseCircleSlotUserWidget::UpdateWidget()
{
	if (IsValid(CP_ParentPanel))
	{
		InitialRotationAxis = FVector(0.f, -1.f, 0.f).RotateAngleAxis(AngleOfFirstWidget, FVector(0.f, 0.f, 1.f));

		TArray<UWidget*> ChildrenWidgetArray = CP_ParentPanel->GetAllChildren();
		NumWidgets = ChildrenWidgetArray.Num();

		uint32 Index = 0;
		for (UPanelSlot* PanelSlot : CP_ParentPanel->GetSlots())
		{
			if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(PanelSlot))
			{
				FAnchors Anchors;
				Anchors.Minimum = { 0.5f, 0.5f };
				Anchors.Maximum = { 0.5f, 0.5f };

				CanvasPanelSlot->SetAnchors(Anchors);
				CanvasPanelSlot->SetAlignment({ 0.5f, 0.5f });


				float AngleDeg = (360.f / NumWidgets) * Index;
				FVector RotatePositionVector = InitialRotationAxis.RotateAngleAxis(AngleDeg, { 0.f, 0.f, 1.f });
				RotatePositionVector *= Radius;

				
				CanvasPanelSlot->SetAlignment({ 0.5f, 0.5f });
				CanvasPanelSlot->SetPosition({RotatePositionVector.X, RotatePositionVector.Y});

				if (bRotateWidgets)
				{
					FRotator Rotator = UKismetMathLibrary::MakeRotFromX(RotatePositionVector);
					ChildrenWidgetArray[Index]->SetRenderTransformAngle(BaseWidgetRotation + Rotator.Yaw);
				}
				else
				{
					ChildrenWidgetArray[Index]->SetRenderTransformAngle(BaseWidgetRotation);
				}

				++Index;
			}
		}
	}
}
