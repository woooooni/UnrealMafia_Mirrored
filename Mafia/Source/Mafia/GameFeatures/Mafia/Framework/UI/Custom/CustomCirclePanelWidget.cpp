// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/UI/Custom/CustomCirclePanelWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"



void UCustomCirclePanelWidget::OnSlotAdded(UPanelSlot* InSlot)
{
	Super::OnSlotAdded(InSlot);
	ArrangeCircleWidgets();
}

void UCustomCirclePanelWidget::OnSlotRemoved(UPanelSlot* InSlot)
{
	Super::OnSlotRemoved(InSlot);
	ArrangeCircleWidgets();
}

TSharedRef<SWidget> UCustomCirclePanelWidget::RebuildWidget()
{
	TSharedRef<SWidget> Result = Super::RebuildWidget();
	ArrangeCircleWidgets();
	return Result;
}



void UCustomCirclePanelWidget::ArrangeCircleWidgets()
{
	NumWidgets = GetChildrenCount();
	InitialRotationAxis = FVector(0.f, -1.f, 0.f).RotateAngleAxis(AngleOfFirstWidget, { 0.f, 0.f, 1.f });

	if (0 < NumWidgets)
	{
		uint32 Index = 0;

		FAnchors Anchor;
		Anchor.Minimum = { 0.5f, 0.5f };
		Anchor.Maximum = { 0.5f, 0.5f };

		for (UPanelSlot* PanelSlot :GetSlots())
		{
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
			{
				if (UWidget* Widget = CanvasSlot->Content)
				{
					CanvasSlot->SetAnchors(Anchor);
					CanvasSlot->SetAlignment({ 0.5f, 0.5f });

					float RotateAngleDeg = (360.f / NumWidgets) * Index;
					FVector RotateVector = InitialRotationAxis.RotateAngleAxis(RotateAngleDeg, { 0.f, 0.f, 1.f });
					FVector RotatePositionVector = RotateVector * Radius;

					CanvasSlot->SetPosition({ RotatePositionVector.X, RotatePositionVector.Y });
					if (bRotateWidgets)
					{
						FRotator Rotator = UKismetMathLibrary::MakeRotFromX(RotateVector);
						Widget->SetRenderTransformAngle(BaseWidgetRotation + Rotator.Yaw);
					}
					else
					{
						Widget->SetRenderTransformAngle(BaseWidgetRotation);
					}
					Index++;
				}
			}
		}
	}
}
