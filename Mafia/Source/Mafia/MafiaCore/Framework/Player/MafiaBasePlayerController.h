// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MafiaBasePlayerController.generated.h"

class AMafiaBaseHUD;
class AMafiaBaseCharacter;
/**
 * 
 */
UCLASS(Abstract)
class MAFIA_API AMafiaBasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	AMafiaBasePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostInitializeComponents() override;
	/** Always called immediately after spawning and reading in replicated properties */
	virtual void PostNetInit() override;
	virtual void BindDelegates() {}
	virtual void UnBindDelegates() {}

	/** Only Study */
	virtual void InitPlayerState() override;
	virtual void SpawnDefaultHUD() override;
	//~ Begin APlayerController interface
	virtual void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;
	//~ End APlayerController interface

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void TickDebug(float DeltaSeconds);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region MatchCheat
private:
	UFUNCTION(Exec)
	void CheatSetMinPlayerCount(const int32 InMinPlayerCount = 1);
	UFUNCTION(Server, Reliable)
	void ServerCheatSetMinPlayerCount(const int32 InMinPlayerCount);
	UFUNCTION(Exec)
	void CheatSetReadyForGame(const bool bReady = true);
#pragma endregion

#pragma region Ability & Vote Cheat
private:
	UFUNCTION(Exec)
	void CheatAssignAbility();

	UFUNCTION(Server, Reliable)
	void ServerReqAssignAbility();

	UFUNCTION(Exec)
	void CheatUseAbility(int32 InPlayerNum);

	UFUNCTION(Exec)
	void CheatVote(int32 InPlayerNum);
#pragma endregion

#pragma region MaterialCheat
	UFUNCTION(Exec)
	void CheatChangePlayerColor(float InRed, float InGreen, float InBlue, float InAlpha);
#pragma endregion


};
