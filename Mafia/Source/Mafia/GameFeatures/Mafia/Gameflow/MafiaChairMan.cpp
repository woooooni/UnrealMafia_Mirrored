// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Gameflow/MafiaChairMan.h"
#include "Mafia/MafiaCore/Framework/System/MafiaLogChannels.h"
#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerState.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "Mafia.h"

UMafiaChairMan::UMafiaChairMan(const FObjectInitializer& ObjectInitializer)
{
	/**
		ktw - TArray를 Heap으로 사용할 경우, Heapify를 먼저 호출해야합니다.
		https://dev.epicgames.com/documentation/ko-kr/unreal-engine/array-containers-in-unreal-engine
	*/

	CachedAbilityEventsHeap.Reserve(16);
	CachedAbilityEventsHeap.Heapify();
}

void UMafiaChairMan::AddAbilityEvent(AMafiaPlayerState* InOrigin, AMafiaPlayerState* InDestination)
{
	FUseAbilityEvent Event;

	if (nullptr == InOrigin || nullptr == InDestination)
	{
		MAFIA_ULOG(LogMafiaManager, Log, TEXT("UMafiaChairMan : InOrigin is Null or InDestination is Null"));
		return;
	}
		

	Event.Role = InOrigin->GetRoleComponent()->GetRoleType();
	Event.Origin = InOrigin->GetRoleComponent();
	Event.Destination = InDestination->GetRoleComponent();

	
	CachedAbilityEventsHeap.HeapPush(Event);
}



void UMafiaChairMan::FlushAbilityEvents()
{
	// 멀티캐스트 델리게이트 or 그냥 쌩 함수 호출?
	for (auto& Event : CachedAbilityEventsHeap)
	{
		if (Event.Destination.IsValid())
		{
			Event.Destination.Get()->ClientAffectedByOther(Event.Role, Event.Origin.Get());
		}
	}
	CachedAbilityEventsHeap.Empty();
}
