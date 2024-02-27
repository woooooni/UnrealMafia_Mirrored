// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Manager/MafiaActorManager.h"
#include "Mafia.h"

AMafiaActorManager::AMafiaActorManager()
{

}

void AMafiaActorManager::PostInitializeComponents()
{
	MAFIA_ALOG(LogMafiaManager, Warning, TEXT("Begin"));
	Super::PostInitializeComponents();
	MAFIA_ALOG(LogMafiaManager, Warning, TEXT("End"));
}

void AMafiaActorManager::PostNetInit()
{
	/** Actor의 bReplicated가 true일때만 호출됨 */
	MAFIA_ALOG(LogMafiaManager, Warning, TEXT("Begin"));
	Super::PostNetInit();
	MAFIA_ALOG(LogMafiaManager, Warning, TEXT("End"));
}

void AMafiaActorManager::BeginPlay()
{
	MAFIA_ALOG(LogMafiaManager, Warning, TEXT("Begin"));
	Super::BeginPlay();
	MAFIA_ALOG(LogMafiaManager, Warning, TEXT("End"));
}