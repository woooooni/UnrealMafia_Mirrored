// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameMode.h"
#include "MafiaGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MAFIA_API AMafiaGameMode final : public AMafiaBaseGameMode
{
	GENERATED_BODY()
	
/** overriding */
private:
	AMafiaGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

	virtual void OnEndAssigningAbilityState() override;



};
