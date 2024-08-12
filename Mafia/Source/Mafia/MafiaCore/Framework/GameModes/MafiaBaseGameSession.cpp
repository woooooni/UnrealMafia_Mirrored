// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/GameModes/MafiaBaseGameSession.h"
#include "Mafia.h"

AMafiaBaseGameSession::AMafiaBaseGameSession()
{

}

void AMafiaBaseGameSession::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
	MAFIA_ALOG(LogMafiaGameSession, Log, TEXT("HandleMatchIsWaitingToStart"));
}

void AMafiaBaseGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	MAFIA_ALOG(LogMafiaGameSession, Log, TEXT("HandleMatchHasStarted"));
}

void AMafiaBaseGameSession::HandleRoundIsAssigningAbility()
{
	MAFIA_ALOG(LogMafiaGameSession, Log, TEXT("HandleRoundIsAssigningAbility"));
}

void AMafiaBaseGameSession::HandleInProgressMafia()
{
	MAFIA_ALOG(LogMafiaGameSession, Log, TEXT("HandleInProgressMafia"));
}

void AMafiaBaseGameSession::HandleWaitingPostRound()
{
	MAFIA_ALOG(LogMafiaGameSession, Log, TEXT("HandleWaitingPostRound"));
}

void AMafiaBaseGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	MAFIA_ALOG(LogMafiaGameSession, Log, TEXT("HandleMatchHasEnded"));
}
