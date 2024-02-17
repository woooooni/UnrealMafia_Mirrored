// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/GameModes/MafiaGameMode.h"
#include "Mafia.h"

AMafiaGameMode::AMafiaGameMode()
{

}

void AMafiaGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMafiaGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMafiaGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMafiaGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AMafiaGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AMafiaGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void AMafiaGameMode::OnEndAssigningAbilityState()
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Called"));

	// 마지막에 호출해주도록 하자
	Super::OnEndAssigningAbilityState();
}
