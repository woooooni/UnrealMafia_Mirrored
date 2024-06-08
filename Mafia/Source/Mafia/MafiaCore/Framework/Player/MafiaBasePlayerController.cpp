// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Player/MafiaBasePlayerController.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/Character/MafiaBaseCharacter.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameMode.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseHUD.h"
#include "Mafia.h"

AMafiaBasePlayerController::AMafiaBasePlayerController(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bEnableClickEvents = true;
	bShowMouseCursor = true;
}

void AMafiaBasePlayerController::PostInitializeComponents()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::PostInitializeComponents();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::InitPlayerState()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::InitPlayerState();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::SpawnDefaultHUD()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::SpawnDefaultHUD();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::PostNetInit()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::PostNetInit();
	BindDelegates();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::BeginPlay()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::BeginPlay();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		TickDebug(DeltaSeconds);
	}
#endif
}

void AMafiaBasePlayerController::TickDebug(float DeltaSeconds)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UWorld* MyWorld = GetWorld();
	if (false == IsValid(MyWorld))
	{
		return;
	}

	AMafiaBaseGameState* GS = MyWorld->GetGameState<AMafiaBaseGameState>();
	if (false == IsValid(GS))
	{
		return;
	}

	AMafiaBasePlayerState* PS = GetPlayerState<AMafiaBasePlayerState>();
	if (false == IsValid(PS))
	{
		return;
	}

	int32 PopupKey = 123456;

	/** GameState:MatchState */
	{
		const FName& CurrentMatchState = GS->GetMatchState();
		const FString DebugString = FString::Printf(TEXT("MatchState:%s, FlowState:%s, Timer:%f"),
			*CurrentMatchState.ToString(), *EnumToStringMafia(EMafiaFlowState, GS->GetMafiaFlowState()), GS->GetGameFlowTimerRemaining());
		GEngine->AddOnScreenDebugMessage(PopupKey--, SMALL_NUMBER, FColor::Black, DebugString);
	}

	/** PlayerState:ReadyForGame */
	{
		const FString DebugString = FString::Printf(TEXT("bReady:%d"), PS->IsReadyForGame());
		GEngine->AddOnScreenDebugMessage(PopupKey--, SMALL_NUMBER, FColor::Black, DebugString);
	}
#endif
}

void AMafiaBasePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::EndPlay(EndPlayReason);
	UnBindDelegates();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));

	// Spawned HUD
	Super::ClientSetHUD_Implementation(NewHUDClass);

	if (AMafiaBaseHUD* HUD = Cast<AMafiaBaseHUD>(MyHUD))
	{
		HUD->InitializeHUD();
	}

	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

#pragma region MatchCheat
void AMafiaBasePlayerController::CheatSetMinPlayerCount(const int32 InMinPlayerCount /*= 1*/)
{
#if ENABLE_CHEAT
	if (ROLE_AutonomousProxy == GetLocalRole())
	{
		ServerCheatSetMinPlayerCount(InMinPlayerCount);
	}
#endif
}

void AMafiaBasePlayerController::ServerCheatSetMinPlayerCount_Implementation(const int32 InMinPlayerCount)
{
	if (AMafiaBaseGameMode* MafiaBaseGameMode = GetWorld()->GetAuthGameMode<AMafiaBaseGameMode>())
	{
		MafiaBaseGameMode->SetGameStartMinPlayerCount(InMinPlayerCount);
	}
}

void AMafiaBasePlayerController::CheatSetReadyForGame(const bool bReady /*= true*/)
{
#if ENABLE_CHEAT
	if (ROLE_AutonomousProxy == GetLocalRole())
	{
		if (AMafiaBasePlayerState* MafiaBasePlayerState = GetPlayerState<AMafiaBasePlayerState>())
		{
			MafiaBasePlayerState->ServerReqReady(bReady);
		}
	}
#endif
}

#pragma endregion|