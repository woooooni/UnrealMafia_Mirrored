// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerController.h"
#include "MafiaPlayerController.generated.h"

class AMafiaCharacter;
/**
 *
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class MAFIA_API AMafiaPlayerController final : public AMafiaBasePlayerController
{
	GENERATED_BODY()


private:
	UFUNCTION(Exec)
	void CheatSetupActor();

	UFUNCTION(Exec)
	void CheatDestoryActor();

	UFUNCTION(Exec)
	void CheatCheckActor();

	AMafiaCharacter* ATestCharacter;
	AMafiaCharacter* BTestCharacter;
};
