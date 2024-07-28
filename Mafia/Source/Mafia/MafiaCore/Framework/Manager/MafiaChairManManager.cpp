// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Manager/MafiaChairManManager.h"
#include "MafiaCore/Framework/System/MafiaLogChannels.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameMode.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "Framework/GameModes/MafiaBaseGameState.h"
#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerState.h"
#include "Algo/RandomShuffle.h"
#include "Mafia.h"


UMafiaChairManManager::UMafiaChairManManager(const FObjectInitializer& ObjectInitializer)
{
	/**
		ktw - TArray를 Heap으로 사용할 경우, Heapify를 먼저 호출해야합니다.
		https://dev.epicgames.com/documentation/ko-kr/unreal-engine/array-containers-in-unreal-engine
	*/
	
	CachedAbilityEventsHeap.Reserve(16);
	CachedAbilityEventsHeap.Heapify();
}



void UMafiaChairManManager::AssigningAllPlayersAbility()
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

void UMafiaChairManManager::AddAbilityEvent(AMafiaBasePlayerState* InOrigin, AMafiaBasePlayerState* InDestination)
{
	FUseAbilityEventData Event;

	if (nullptr == InOrigin || nullptr == InDestination)
	{
		MAFIA_ULOG(LogMafiaChairMan, Log, TEXT("UMafiaChairManManager::AddAbilityEvent : InOrigin is Null or InDestination is Null"));
		return;
	}
		

	Event.Role = InOrigin->GetRoleComponent()->GetRoleType();
	Event.Origin = InOrigin->GetRoleComponent();
	Event.Destination = InDestination->GetRoleComponent();

	if (Event.Origin.IsValid() == false || Event.Destination.IsValid() == false)
	{
		MAFIA_ULOG(LogMafiaChairMan, Log, TEXT("UMafiaChairManManager::AddAbilityEvent : RoleComponent is Null"));
		return;
	}

	
	CachedAbilityEventsHeap.HeapPush(Event);
}



void UMafiaChairManManager::DispatchAbilityEvents()
{
	for (auto& Event : CachedAbilityEventsHeap)
	{
		if (Event.Origin.IsValid() && Event.Destination.IsValid())
		{
			Event.Destination.Get()->AffectedAbilityByOther(Event.Role, Event.Origin.Get());

			/** Todo - ktw :  Response 신호를 언제 뿌려줄지 결정.(ex) 경찰이면, 경찰한테'만' 찍은 플레이어의 팀을 알려줘야 한다.) */
			Event.Origin.Get()->ResponsePostUseAbility(Event.Destination.Get());
		}
	}
	CachedAbilityEventsHeap.Empty();
}

void UMafiaChairManManager::FlushAbilityEvents() const
{
	if (UWorld* World = GetWorld())
	{
		if (AMafiaBaseGameState* GS = World->GetGameState<AMafiaBaseGameState>())
		{
			for (auto& Pair : GS->GetJoinedUserPlayerStateMap())
			{
				if (Pair.Value.IsValid())
				{
					if (AMafiaBasePlayerState* PS = Pair.Value.Get())
					{
						if (UMafiaBaseRoleComponent* RoleComponent = PS->GetRoleComponent())
						{
							RoleComponent->FlushAbilityEvents();
						}
					}
				}
				else
				{
					MAFIA_ULOG(LogMafiaChairMan, Error, TEXT("PlayerState Is Invalid."));
				}
			}
		}
	}
}

void UMafiaChairManManager::StartVote()
{
	if (UWorld* World = GetWorld())
	{
		if (AMafiaBaseGameState* GS = World->GetGameState<AMafiaBaseGameState>())
		{
			for (auto& Pair : GS->GetJoinedUserPlayerStateMap())
			{
				if (Pair.Value.IsValid())
				{
					if (AMafiaBasePlayerState* PS = Pair.Value.Get())
					{
						if (UMafiaBaseRoleComponent* RoleComponent = PS->GetRoleComponent())
						{
							RoleComponent->PreVoteEvent();
						}
					}
				}
				else
				{
					MAFIA_ULOG(LogMafiaChairMan, Error, TEXT("PlayerState Is Invalid."));
				}
			}
		}
	}
}

void UMafiaChairManManager::AddVoteEvent(AMafiaBasePlayerState* InVotor, AMafiaBasePlayerState* InCandidate)
{
	if (IsValid(InVotor) && IsValid(InCandidate))
	{
		/** ktw : 1. AccountId & RoleComponent 유효성 체크. */
		const FString& VotorAccountIdStr = InVotor->GetUniqueId().ToString();
		const FString& CandidateAccountIdStr = InCandidate->GetUniqueId().ToString();

		if (CandidateAccountIdStr.IsEmpty() || VotorAccountIdStr.IsEmpty())
		{
			MAFIA_ULOG(LogMafiaChairMan, Warning, TEXT("AccountIdStr is empty"));
			return;
		}

		UMafiaBaseRoleComponent* VotorRoleComponent = InVotor->GetRoleComponent();
		UMafiaBaseRoleComponent* CandidateRoleComponent = InCandidate->GetRoleComponent();

		if (IsValid(VotorRoleComponent) == false || IsValid(CandidateRoleComponent) == false)
		{
			MAFIA_ULOG(LogMafiaChairMan, Warning, TEXT("RoleComponent is Invalid."));
			return;
		}


		/** ktw : 2. 중복 투표 체크. */
		const FName VotorAccountId = FName(*VotorAccountIdStr);
		const FName CandidateAccountId = FName(*CandidateAccountIdStr);
		EMafiaVoteFlag Flag = EMafiaVoteFlag::ImpossibleVote;
		
		if (auto Pair = CachedVoteEventsMap.Find(CandidateAccountId))
		{
			if (Pair->VotersSet.Find(VotorAccountId))
			{
				Flag = EMafiaVoteFlag::AlreadyVoted;
				VotorRoleComponent->ResponseVoteEvent(CandidateRoleComponent, Flag);
				return;
			}
		}


		/** ktw : 3. 투표가 가능한 상태인지 체크. */
		if (IsPossibleVote())
		{
			FPlayerVoteData* Pair = CachedVoteEventsMap.Find(CandidateAccountId);
			if (Pair)
			{
				/** Todo - ktw : 필요하면, 예외처리 추가. */
				Pair->VotersSet.Emplace(VotorAccountId);
				Pair->VotedCount++;
			}
			else
			{
				FPlayerVoteData VoteData;
				VoteData.VotersSet.Emplace(VotorAccountId);
				VoteData.VotedCount = 1;
				VoteData.Candidate = CandidateRoleComponent;
				
				CachedVoteEventsMap.Emplace(CandidateAccountId, VoteData);
			}

			Flag = EMafiaVoteFlag::Succeed;
		}

		VotorRoleComponent->ResponseVoteEvent(CandidateRoleComponent, Flag);
	}
}

void UMafiaChairManManager::EndVote()
{
	CachedVoteEventsMap.Empty();
}



bool UMafiaChairManManager::MakeShuffledRoleArray(int32 InUserCount, OUT TArray<EMafiaRole>& OutSuffledArray)
{
	/** 
	OutSuffledArray.Reserve(InUserCount);

	/** 
		ktw - 플레이어 수 : 4 ~ 20
		FMath::IsWithin				: 이상 ~ 미만
		FMath::IsWithinInclusive	: 이상 ~ 이하
	*/
	if (FMath::IsWithinInclusive<int32>(InUserCount, 1, 12) == false)
	{
		return false;
	}
		

	TArray<EMafiaTeam> DistributionArray = TArray<EMafiaTeam>(GTeamDistributionArray.GetData(), InUserCount);
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

bool UMafiaChairManManager::IsPossibleVote()
{
	if (UWorld* World = GetWorld())
	{
		if (AMafiaBaseGameState* GS = World->GetGameState<AMafiaBaseGameState>())
		{
			return EMafiaFlowState::Vote == GS->GetMafiaFlowState();
		}
	}
	return false;
}

void UMafiaChairManManager::OnSetMafiaFlowState(EMafiaFlowState InFlowState)
{
	/** ktw : AMafiaBaseGameState::SetMafiaFlowState에서 호출됩니다. */
	
	if (EMafiaFlowState::None == InFlowState)
	{
		AssigningAllPlayersAbility();
	}
	else if (EMafiaFlowState::Day == InFlowState)
	{
		/** 
			Todo - ktw : Reliable RPC 라도 리플리케이션되는데 시간이 걸려서 Distpatch 후 따로 클라이언트에게 신호를 받고 
							Flush 신호를 보내는 걸로 넘어가야 할 것 같기도 합니다. (그때 동안 연출.)
		*/

		DispatchAbilityEvents();
		FlushAbilityEvents();
	}	
	else if (EMafiaFlowState::Vote == InFlowState)
	{
		StartVote();
	}
	else if (EMafiaFlowState::Night == InFlowState)
	{
		EndVote();
	}
			
}
