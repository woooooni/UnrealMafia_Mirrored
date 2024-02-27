// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseUserWidget.h"
#include "MafiaBaseActionGroupWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class MAFIA_API UMafiaBaseActionGroupWidget : public UMafiaBaseUserWidget
{
	GENERATED_BODY()
	
protected:
	UMafiaBaseActionGroupWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BindDelegates() override;
	virtual void UnBindDelegates() override;
};
