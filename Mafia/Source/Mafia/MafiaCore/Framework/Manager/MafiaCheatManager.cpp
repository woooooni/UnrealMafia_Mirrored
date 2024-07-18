// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Manager/MafiaCheatManager.h"
#include "Framework/System/MafiaBaseGameInstance.h"
#include "Framework/Manager/MafiaChairManManager.h"
#include "Framework/GameModes/MafiaBaseGameState.h"

void UMafiaCheatManager::CheatTemplate()
{
	GEngine->AddOnScreenDebugMessage(32, 3.f, FColor::Red, FString::Printf(TEXT("CheatTemplate")));
}

void UMafiaCheatManager::PlatFormCMD(const FString& InCommand)
{
	GEngine->AddOnScreenDebugMessage(32, 10.f, FColor::Red, FString::Printf(TEXT("Platform CommandLine : %s"), *InCommand));
	//if (const UMafiaClientPlatformSessionSubsystem* SessionSubsystem = UMafiaClientPlatformSessionSubsystem::Get(this))
	//{
	//	const UMafiaLobbySession* LobbySession = SessionSubsystem->GetSession<UMafiaLobbySession>();
	//	if (true == IsValid(LobbySession))
	//	{
	//		LobbySession->Req_Cheat(InCommand);
	//	}
	//}
}

