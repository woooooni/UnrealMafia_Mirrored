// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseUserWidget.h"
#include "MafiaAbilityPlayerCardUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaAbilityPlayerCardUserWidget : public UMafiaBaseUserWidget
{
	GENERATED_BODY()

private:
	UMafiaAbilityPlayerCardUserWidget(const FObjectInitializer& ObjectInitializer);
	
public:
	bool InitializePlayer(class AMafiaBasePlayerState* InPlayerState);

public:
	FORCEINLINE class AMafiaBasePlayerState* GetOwnerPlayer() { return OwnerPlayer.Get(); }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;


private:
	UFUNCTION()
	void OnClickedPlayerAbilityCard();


private:
	TWeakObjectPtr<class AMafiaBasePlayerState> OwnerPlayer;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UButton> BTN_AbilityPlayerCard;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> IMG_PlayerColor;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> IMG_AbilityIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TB_PlayerColor;

	
};
