// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFeatures/Mafia/Framework/UI/Viewport/ChairMan/MafiaChairmanLogScrollUserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

UMafiaChairmanLogScrollUserWidget::UMafiaChairmanLogScrollUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VisibleTextIndex = 0;
}

void UMafiaChairmanLogScrollUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	PlayAnimation(MouseOnAnimation);
}

void UMafiaChairmanLogScrollUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	PlayAnimation(MouseLeaveAnimation);
}

void UMafiaChairmanLogScrollUserWidget::AddLogText(const FString& InLogStr)
{
	if (IsValid(SB_Log))
	{
		const TArray<UWidget*>& ChildrenWidgets = SB_Log->GetAllChildren();
		if (ChildrenWidgets.IsValidIndex(VisibleTextIndex))
		{
			if (UTextBlock* TextBlock = Cast<UTextBlock>(ChildrenWidgets[VisibleTextIndex]))
			{
				TextBlock->SetText(FText::FromString(InLogStr));
				VisibleTextIndex++;
			}
		}
		else
		{
			// Err
		}

	}
}
