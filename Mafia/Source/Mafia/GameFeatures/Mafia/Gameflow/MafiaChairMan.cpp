// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Gameflow/MafiaChairMan.h"
#include "Mafia/MafiaCore/Framework/System/MafiaLogChannels.h"
#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerState.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "Framework/GameModes/MafiaBaseGameState.h"
#include "Algo/RandomShuffle.h"
#include "Mafia.h"


UMafiaChairMan::UMafiaChairMan(const FObjectInitializer& ObjectInitializer)
{
	/**
		ktw - TArray를 Heap으로 사용할 경우, Heapify를 먼저 호출해야합니다.
		https://dev.epicgames.com/documentation/ko-kr/unreal-engine/array-containers-in-unreal-engine
	*/
	
	CachedAbilityEventsHeap.Reserve(16);
	CachedAbilityEventsHeap.Heapify(FUseAbilityEvent::StaticStruct());
}

void UMafiaChairMan::AssigningAbilities()
{
	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		if (AMafiaBaseGameState* GameState = World->GetGameState<AMafiaBaseGameState>())
		{
			int32 PlayerCount = GameState->GetJoinedUserCount();
			TArray<EMafiaRole> ShuffledRoleArray;

			bool bMakeSucceed = MakeShuffledRoleArray(PlayerCount, ShuffledRoleArray);
			if (bMakeSucceed)
			{
				int32 Num = 0;
				for (auto& Iter : GameState->GetJoinedUserPlayerStateMap())
				{
					if (Iter.Value.IsValid())
					{
						Iter.Value.Get()->AssignAbility(ShuffledRoleArray[Num++]);
					}
				}
			}
		}
	}
}

void UMafiaChairMan::AddAbilityEvent(AMafiaBasePlayerState* InOrigin, AMafiaBasePlayerState* InDestination)
{
	FUseAbilityEvent Event;

	if (nullptr == InOrigin || nullptr == InDestination)
	{
		MAFIA_ULOG(LogMafiaManager, Log, TEXT("UMafiaChairMan::AddAbilityEvent : InOrigin is Null or InDestination is Null"));
		return;
	}
		

	Event.Role = InOrigin->GetRoleComponent()->GetRoleType();
	Event.Origin = InOrigin->GetRoleComponent();
	Event.Destination = InDestination->GetRoleComponent();

	if (Event.Origin.IsValid() == false || Event.Destination.IsValid() == false)
	{
		MAFIA_ULOG(LogMafiaManager, Log, TEXT("UMafiaChairMan::AddAbilityEvent : RoleComponent is Null"));
		return;
	}

	
	CachedAbilityEventsHeap.HeapPush(Event);
}



void UMafiaChairMan::FlushAbilityEvents()
{
	for (auto& Event : CachedAbilityEventsHeap)
	{
		if (Event.Destination.IsValid())
		{
			Event.Destination.Get()->AffectedByOther(Event.Role, Event.Origin.Get());
		}
	}
	CachedAbilityEventsHeap.Empty();
}


bool UMafiaChairMan::MakeShuffledRoleArray(int32 InUserCount, OUT TArray<EMafiaRole>& OutSuffledArray)
{
	/** 
	OutSuffledArray.Reserve(InUserCount);

	/** 
		ktw - 플레이어 수 : 4 ~ 20
		FMath::IsWithin				: 이상 ~ 미만
		FMath::IsWithinInclusive	: 이상 ~ 이하
	*/
	if (FMath::IsWithinInclusive<int32>(InUserCount, 4, 12) == false)
	{
		return false;
	}
		

	TArray<EMafiaTeam> DistributionArray = TArray<EMafiaTeam>(GTeamDistributionArray.GetData(), InUserCount - 1);
	Algo::RandomShuffle<TArray<EMafiaTeam>>(DistributionArray);


	int32 CitizenIdx = 0;
	int32 MafiaIdx = 0;

	for (int32 i = 0; i < InUserCount; ++i)
	{
		switch (DistributionArray[i])
		{
		case EMafiaTeam::Citizen:
			OutSuffledArray.Push(GCitizenAssignRoleArray[CitizenIdx]);
			++CitizenIdx;
			break;
		case EMafiaTeam::Mafia:
			OutSuffledArray.Push(GMafiaAssignRoleArray[MafiaIdx]);
			++MafiaIdx;
			break;
		case EMafiaTeam::Neutral:
			OutSuffledArray.Push(EMafiaRole::Killer);
			break;
		}
	}
	
	Algo::RandomShuffle<TArray<EMafiaRole>>(OutSuffledArray);
	return true;
}
