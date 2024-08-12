// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "MafiaBaseGameSession.generated.h"

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class MAFIA_API AMafiaBaseGameSession : public AGameSession
{
	GENERATED_BODY()
	
protected:
	AMafiaBaseGameSession();

public:
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleRoundIsAssigningAbility();
	virtual void HandleInProgressMafia();
	virtual void HandleWaitingPostRound();
	virtual void HandleMatchHasEnded() override;
};
