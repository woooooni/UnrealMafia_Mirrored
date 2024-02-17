// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/System/MafiaGameEvents.h"
#include "Framework/System/MafiaBaseGameInstance.h"
#include "Kismet/GameplayStatics.h"

UMafiaGameEvents* UMafiaGameEventsLibrary::GetGameEvents(const UObject* WorldContextObject)
{
	if (IsValid(WorldContextObject))
	{
		UMafiaBaseGameInstance* Instance = Cast<UMafiaBaseGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (IsValid(Instance))
		{
			return Instance->GetGameEvents();
		}
	}

	return nullptr;
}

bool UMafiaGameEventsLibrary::IsGameEventsValid(const UObject* WorldContextObject)
{
	return (GetGameEvents(WorldContextObject) != nullptr);
}
