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



bool UMafiaChairManManager::AssigningAllPlayersAbility()
{
	UWorld* World = GetWorld();
	JoinedPlayerRoleComponents.Empty();

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
				for (auto& Pair : GameState->GetJoinedUserPlayerStateMap())
				{
					if (Pair.Value.IsValid())
					{
						if (AMafiaBasePlayerState* PS = Pair.Value.Get())
						{
							const FString& AccountIdStr = PS->GetUniqueId().ToString();
							if (AccountIdStr.IsEmpty())
							{
								MAFIA_ULOG(LogMafiaChairMan, Warning, TEXT("AccountIdStr is empty"));
								return false;
							}

							const FName AccountId = FName(*AccountIdStr);
							if (JoinedPlayerRoleComponents.Find(AccountId))
							{
								return false;
							}
							else
							{
								UMafiaBaseRoleComponent* NewRoleComponent = Pair.Value.Get()->AssignAbility(ShuffledRoleArray[Num++]);
								JoinedPlayerRoleComponents.Emplace(AccountId, NewRoleComponent);
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
			}

			return true;
		}
	}

	return false;
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

void UMafiaChairManManager::CheatChangeRole(AMafiaBasePlayerState* InPlayerState, UMafiaBaseRoleComponent* InNewRoleComponent)
{
#if ENABLE_CHEAT
	const FString& AccountIdStr = InPlayerState->GetUniqueId().ToString();
	if (AccountIdStr.IsEmpty())
	{
		MAFIA_ULOG(LogMafiaChairMan, Warning, TEXT("AccountIdStr is empty"));
		return;
	}

	const FName AccountId = FName(*AccountIdStr);
	if (auto Pair = JoinedPlayerRoleComponents.Find(AccountId))
	{
		JoinedPlayerRoleComponents.Add(AccountId, InNewRoleComponent);
	}
#endif
}

void UMafiaChairManManager::DispatchAbilityEvents()
{
	/** Todo - ktw :  버스드라이버, 기생 처리 */
	while (!CachedAbilityEventsHeap.IsEmpty())
	{
		FUseAbilityEventData Event;
		CachedAbilityEventsHeap.HeapPop(Event);
		
		if (Event.Origin.IsValid() && Event.Destination.IsValid())
		{
			/** Todo - ktw :  Response 신호를 언제 뿌려줄지 결정.(ex) 경찰이면, 경찰한테'만' 찍은 플레이어의 팀을 알려줘야 한다.) */
			Event.Destination.Get()->AffectedAbilityByOther(Event.Role, Event.Origin.Get());
			Event.Origin.Get()->ResponsePostUseAbility(Event.Destination.Get());
		}
	}

	CachedAbilityEventsHeap.Empty();
}

void UMafiaChairManManager::FlushAbilityEvents() const
{
	for (auto& Pair : JoinedPlayerRoleComponents)
	{
		Pair.Value->AffectedEventsFlush();
	}
}

void UMafiaChairManManager::StartVote()
{
	CachedVoteEventsMap.Empty();
	for (auto& Pair : JoinedPlayerRoleComponents)
	{
		Pair.Value->StartVoteEvent();
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
	for (auto& Pair : JoinedPlayerRoleComponents)
	{
		Pair.Value->FinishVoteEvent();
	}

	CachedVoteEventsMap.Empty();
}



bool UMafiaChairManManager::MakeShuffledRoleArray(int32 InUserCount, OUT TArray<EMafiaRole>& OutSuffledArray)
{
	/** 
	OutSuffledArray.Reserve(InUserCount);

	/** 
		ktw - 플레이어 수 : 4 ~ 20 (임시로 1명도 플레이 가능.)
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

EMafiaGameResult UMafiaChairManManager::CheckGameOver() const
{
	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		if (AMafiaBaseGameState* GameState = World->GetGameState<AMafiaBaseGameState>())
		{
			int8 AliveCitizenCount = 0;
			int8 AliveMafiaCount = 0;

			for (auto& Pair : JoinedPlayerRoleComponents)
			{
				/** 
					ktw : 마피아팀 승리조건 체크. 
					1순위 : 마피아팀 -> 2순위 : 시민팀 -> 3순위 : 중립
				*/
				if (UMafiaBaseRoleComponent* RoleComponent = Pair.Value.Get())
				{
					if (RoleComponent->IsDead() == false)
					{
						EMafiaTeam TeamType = RoleComponent->GetTeamType();
						if (TeamType == EMafiaTeam::Citizen)
						{
							++AliveCitizenCount;
						}
						else if (TeamType == EMafiaTeam::Mafia)
						{
							++AliveMafiaCount;
						}
					}
				}
			}

			if (AliveMafiaCount >= AliveCitizenCount)
			{
				return EMafiaGameResult::MafiaWin;
			}
			else if (AliveMafiaCount == 0)
			{
				if (AliveCitizenCount == 0)
				{
					return EMafiaGameResult::NeutralWin;
				}
				else
				{
					return EMafiaGameResult::CitizenWin;
				}
			}
			else
			{
				return EMafiaGameResult::None;
			}
		}
	}

	return EMafiaGameResult::Invalid;
}

void UMafiaChairManManager::NotifyGameResult(EMafiaGameResult InGameResult) const
{
	if (InGameResult == EMafiaGameResult::None)
	{
		return;
	}
		
	if (UWorld* World = GetWorld())
	{
		if (AMafiaBaseGameState* GS = World->GetGameState<AMafiaBaseGameState>())
		{
			for (auto& Pair : GS->GetJoinedUserPlayerStateMap())
			{
				if (AMafiaBasePlayerState* PS = Pair.Value.Get())
				{
					PS->NotifyGameResult(InGameResult);
				}
			}
		}
	}
}

void UMafiaChairManManager::OnSetMafiaFlowState(EMafiaFlowState InFlowState)
{
	/** ktw : AMafiaBaseGameState::SetMafiaFlowState에서 호출됩니다. */
	if (EMafiaFlowState::None == InFlowState)
	{
		CachedAbilityEventsHeap.Empty();
		CachedVoteEventsMap.Empty();
	}
	else if (EMafiaFlowState::DayBefore == InFlowState)
	{
		EMafiaGameResult Result = CheckGameOver();
		NotifyGameResult(Result);
	}
	else if (EMafiaFlowState::Day == InFlowState)
	{
		DispatchAbilityEvents();
		FlushAbilityEvents();
	}
	else if (EMafiaFlowState::DayAfter == InFlowState)
	{
		CachedAbilityEventsHeap.Empty();
		CachedVoteEventsMap.Empty();
	}
	else if (EMafiaFlowState::VoteBefore == InFlowState)
	{
		StartVote();
	}
	else if (EMafiaFlowState::Vote == InFlowState)
	{
		
	}
	else if (EMafiaFlowState::VoteAfter == InFlowState)
	{
		EndVote();

		EMafiaGameResult Result = CheckGameOver();
		NotifyGameResult(Result);
	}
	else if (EMafiaFlowState::NightBefore == InFlowState)
	{
		
	}
	else if (EMafiaFlowState::Night == InFlowState)
	{
		
	}
	else if (EMafiaFlowState::NightAfter == InFlowState)
	{
		EMafiaGameResult Result = CheckGameOver();
		NotifyGameResult(Result);
	}
			
}
