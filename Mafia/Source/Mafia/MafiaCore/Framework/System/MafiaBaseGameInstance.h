// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MafiaBaseGameInstance.generated.h"

class UMafiaWorldDataManager;
class UMafiaGameEvents;
class UMafiaCheatManager;
/**
 * 
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class MAFIA_API UMafiaBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	static UMafiaBaseGameInstance* Get(const UObject* WorldContextObject);
	static UMafiaWorldDataManager* GetMafiaWorldDataManager(const UObject* WorldContextObject);

public:
	/* [OnlyEditor] Called to initialize the game instance for PIE instances of the game */
	/** @See UEditorEngine::CreateInnerProcessPIEGameInstance() 에서 호출됨. */
	virtual FGameInstancePIEResult InitializeForPlayInEditor(int32 PIEInstanceIndex, const FGameInstancePIEParameters& Params) override;

	virtual void Init() override;

	/* [OnlyEditor] Called to actually start the game when doing Play/Simulate In Editor */
	/** 내부에서 GameMode InitGame 호출함. @See PlayWorld->InitializeActorsForPlay(URL); */
	virtual FGameInstancePIEResult StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params) override;

	virtual void OnStart() override;

	/** Call to create the game mode for a given map URL */
	virtual class AGameModeBase* CreateGameModeForURL(FURL InURL, UWorld* InWorld) override;

	virtual void Shutdown() override;
	virtual void StartGameInstance() override;

	/** Only Cheat
	* This function handles a console exec sent to the object; it is virtual so 'nexus' objects like
	* a player controller can reroute the command to several different objects.
	*/
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;
public:
	UMafiaGameEvents* GetGameEvents() const;

private:
	void OnPreLoadMap(const FString& PreloadMapPath);
	void OnPostLoadMap(UWorld* InWorld);

	void OnConnectionFailed(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

private:
	UPROPERTY(Transient)
	UMafiaGameEvents* GameEvents;

	UPROPERTY(Transient)
	UMafiaCheatManager* MafiaCheatManager;
};
