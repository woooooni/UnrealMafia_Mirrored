// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaGameState.generated.h"

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class MAFIA_API AMafiaGameState final : public AMafiaBaseGameState
{
	GENERATED_BODY()
	
private:
	AMafiaGameState();

};
