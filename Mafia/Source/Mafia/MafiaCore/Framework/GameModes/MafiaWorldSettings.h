// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "MafiaWorldSettings.generated.h"

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class MAFIA_API AMafiaWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
protected:
	explicit AMafiaWorldSettings(const FObjectInitializer& ObjectInitializer);

};
