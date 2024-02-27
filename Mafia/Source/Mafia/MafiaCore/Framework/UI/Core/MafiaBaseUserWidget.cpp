// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/UI/Core/MafiaBaseUserWidget.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseHUD.h"
#include "MafiaCore/Framework/Character/MafiaBaseCharacter.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerController.h"
#include "Mafia.h"

UMafiaBaseUserWidget::UMafiaBaseUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMafiaBaseUserWidget::NativeConstruct()
{
	MAFIA_ULOG(LogMafiaWidget, Log, TEXT("Begin"));
	Super::NativeConstruct();
	MAFIA_ULOG(LogMafiaWidget, Log, TEXT("End"));
}

void UMafiaBaseUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UMafiaBaseUserWidget::Initialize()
{
	MAFIA_ULOG(LogMafiaWidget, Log, TEXT("Begin"));
	if (Super::Initialize())
	{
		if (!IsDesignTime())
		{
			BindDelegates();
		}
		MAFIA_ULOG(LogMafiaWidget, Log, TEXT("End"));
		return true;
	}
	MAFIA_ULOG(LogMafiaWidget, Log, TEXT("End"));
	return false;
}

void UMafiaBaseUserWidget::BeginDestroy()
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		Finalize();
	}

	Super::BeginDestroy();
}

void UMafiaBaseUserWidget::Finalize()
{
	if (!IsDesignTime())
	{
		UnBindDelegates();
	}
}