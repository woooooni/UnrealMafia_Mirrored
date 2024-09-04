// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseActionGroupWidget.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaPlayerNameActionGroupWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaPlayerNameActionGroupWidget : public UMafiaBaseActionGroupWidget
{
	GENERATED_BODY()
	
private:
	UMafiaPlayerNameActionGroupWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetPlayerName(const FName& InNewPlayerName);
	void SetFontSize(const float InFontSize, const float InDeltaTime);
	void SetTextColor(const FLinearColor& InColor);


private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_PlayerName;
};
