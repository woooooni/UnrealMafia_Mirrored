// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/GameModes/MafiaWorldSettings.h"
#include "MafiaCore/Framework/Manager/MafiaWorldDataManager.h"
#include "Mafia.h"

AMafiaWorldSettings::AMafiaWorldSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AMafiaWorldSettings::PostInitializeComponents()
{
	MAFIA_ALOG(LogTemp, Warning, TEXT("Begin"));
	Super::PostInitializeComponents();
	MAFIA_ALOG(LogTemp, Warning, TEXT("End"));

	if (GetNetMode() == ENetMode::NM_Standalone)
	{
		return;
	}

	if (IsValid(MafiaWorldDataManagerClass))
	{
		MafiaWorldDataManager = NewObject<UMafiaWorldDataManager>(this, MafiaWorldDataManagerClass);
		MafiaWorldDataManager->Initialize();
	}
	else
	{
		ensureMsgf(false, TEXT("MafiaWorldDataManagerClass is invalid. please check MafiaWorldDataManagerClass"));
	}
}