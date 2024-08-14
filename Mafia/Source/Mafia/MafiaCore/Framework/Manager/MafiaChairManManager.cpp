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
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		if (AMafiaBaseGameState* GS = World->GetGameState<AMafiaBaseGameState>())
		{
			if (IsValid(InOrigin) && IsValid(InDestination))
			{
				FUseAbilityEventData Event;

				Event.Role = InOrigin->GetRoleComponent()->GetRoleType();
				Event.Origin = InOrigin->GetRoleComponent();
				Event.Destination = InDestination->GetRoleComponent();

				if (Event.Origin.IsValid() == false || Event.Destination.IsValid() == false)
				{
					MAFIA_ULOG(LogMafiaChairMan, Log, TEXT("UMafiaChairManManager::AddAbilityEvent : RoleComponent is Invalid."));
					return;
				}

				/** 
					#Todo - ktw : 밤에만 능력사용 가능. 임시로 막아둠.
				*/

				//if (EMafiaFlowState::Night != GS->GetMafiaFlowState())
				//{
				//	MAFIA_ULOG(LogMafiaChairMan, Log, TEXT("UMafiaChairManManager::AddAbilityEvent : Abilities can only be used at Night"));
				//	Event.Origin->ResponseUseAbility(InDestination->GetRoleComponent(), EMafiaAbilityFlag::ImpossibleUseAbility);
				//	return;
				//}
				

				if (CachedAbilityEventsHeap.Find(Event))
				{
					MAFIA_ULOG(LogMafiaChairMan, Log, TEXT("UMafiaChairManManager::AddAbilityEvent : Already Use Ability."));
					Event.Origin->ResponseUseAbility(InDestination->GetRoleComponent(), EMafiaUseAbilityFlag::AlreadyUseAbility);
					return;
				}
				else
				{
					CachedAbilityEventsHeap.HeapPush(Event);
					Event.Origin->ResponseUseAbility(InDestination->GetRoleComponent(), EMafiaUseAbilityFlag::Succeed);
					return;
				}
			}
			else
			{
				MAFIA_ULOG(LogMafiaChairMan, Log, TEXT("UMafiaChairManManager::AddAbilityEvent : InOrigin is Invalid or InDestination is Invalid."));
				return;
			}
		}
	}
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
			if (Event.Role == EMafiaRole::BusDriver)
			{
				Event.Origin.Get()->BusDrive();
			}
			else
			{
				Event.Destination.Get()->AffectedAbilityByOther(Event.Role, Event.Origin.Get());
			}
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
	CachedAbilityEventsHeap.Empty();
	CachedVoteEventsMap.Empty();
	CachedAlreadyVotersSet.Empty();

	for (auto& Pair : JoinedPlayerRoleComponents)
	{
		Pair.Value->StartVoteEvent();
	}
}

UMafiaBaseRoleComponent* UMafiaChairManManager::FindDeathRow()
{
	/** ktw : 투표로 처형할 사람을 찾습니다. */
	
	/** 
		1. 무효표 수 계산. 무효 데이터 수 한 개 빼야 함.
			(전체 플레이어 수 - 투표한 사람 수 + 1)
	*/
	uint16 NullVoteCount = JoinedPlayerRoleComponents.Num() - (CachedVoteEventsMap.Num() + 1);

	TArray<FPlayerVoteData> VoteArray;
	CachedVoteEventsMap.GenerateValueArray(VoteArray);

	/** 2. 가장 득표수가 많은 Data를 찾고, 무효표 수와 비교해 처형할 사람을 정한다. */
	if (VoteArray.Num() > 0)
	{
		VoteArray.Sort([](const FPlayerVoteData& Left, const FPlayerVoteData& Right)
		{
			return Left.VotedCount > Right.VotedCount;
		});


		FPlayerVoteData MaxCountVoteData;
		for (auto Event : VoteArray)
		{
			if (Event.Candidate.IsValid())
			{
				MaxCountVoteData = Event.VotedCount > MaxCountVoteData.VotedCount ? Event : MaxCountVoteData;
			}
			else
			{
				NullVoteCount += Event.VotedCount;
			}
		}

		
		if (NullVoteCount >= MaxCountVoteData.VotedCount)
		{
			/** 2-1. 무효표와 같거나 적다 */
			return nullptr;
		}

		FPlayerVoteData SecondaryCountVoteData = VoteArray.Num() > 2 ? VoteArray[1] : FPlayerVoteData();
		if (SecondaryCountVoteData.VotedCount == MaxCountVoteData.VotedCount)
		{
			/** 2-2. 가장 많은 득표수와 그 다음 득표수가 같다. */
			return nullptr;
		}

		if (MaxCountVoteData.Candidate.IsValid())
		{
			return MaxCountVoteData.Candidate.Get();
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
}

void UMafiaChairManManager::NotifyDeathRow()
{
	UMafiaBaseRoleComponent* DeathRow = FindDeathRow();
	EMafiaVoteResultFlag Flag = IsValid(DeathRow) ? EMafiaVoteResultFlag::SomeoneDying : EMafiaVoteResultFlag::NoDeathPlayer;
	
	for (auto& Pair : JoinedPlayerRoleComponents)
	{
		Pair.Value->ReceiveVoteResult(DeathRow, Flag);
	}
	
}

void UMafiaChairManManager::AddVoteEvent(AMafiaBasePlayerState* InVotor, AMafiaBasePlayerState* InCandidate)
{
	if (IsValid(InVotor))
	{
		/** ktw : 1. AccountId & RoleComponent 유효성 체크. */
		const FString& VotorAccountIdStr = InVotor->GetUniqueId().ToString();

		if (VotorAccountIdStr.IsEmpty())
		{
			MAFIA_ULOG(LogMafiaChairMan, Warning, TEXT("AccountIdStr is empty"));
			return;
		}

		UMafiaBaseRoleComponent* VotorRoleComponent = InVotor->GetRoleComponent();
		if (IsValid(VotorRoleComponent) == false)
		{
			MAFIA_ULOG(LogMafiaChairMan, Warning, TEXT("RoleComponent is Invalid."));
			return;
		}


		/** ktw : 2. 중복 투표 체크. */
		const FName VotorAccountId = FName(*VotorAccountIdStr);
		if (CachedAlreadyVotersSet.Find(VotorAccountId))
		{
			VotorRoleComponent->ResponseVoteEvent(InCandidate, EMafiaVoteFlag::AlreadyVoted);
			return;
		}


		/** ktw : 3. 투표가 가능한 상태인지 체크. */
		if (IsPossibleVote())
		{
			if (IsValid(InCandidate))
			{
				/** ktw : 피 투표자가 존재할 경우. */
				const FString & CandidateAccountIdStr = InCandidate->GetUniqueId().ToString();
				if (CandidateAccountIdStr.IsEmpty())
				{
					MAFIA_ULOG(LogMafiaChairMan, Warning, TEXT("AccountIdStr is empty"));
					return;
				}
				const FName CandidateAccountId = FName(*CandidateAccountIdStr);

				
				UMafiaBaseRoleComponent* CandidateRoleComponent = InCandidate->GetRoleComponent();
				if (IsValid(CandidateRoleComponent))
				{
					FPlayerVoteData* Pair = CachedVoteEventsMap.Find(CandidateAccountId);
					if (Pair)
					{
						Pair->VotedCount++;
						CachedAlreadyVotersSet.Emplace(VotorAccountId);
					}
					else
					{
						FPlayerVoteData VoteData;
						VoteData.Candidate = CandidateRoleComponent;
						VoteData.VotedCount = 1;

						CachedAlreadyVotersSet.Emplace(VotorAccountId);
						CachedVoteEventsMap.Emplace(CandidateAccountId, VoteData);
					}
				}
				else
				{
					VotorRoleComponent->ResponseVoteEvent(InCandidate, EMafiaVoteFlag::ImpossibleVote);
					return;
				}
			}
			else
			{
				/** ktw : 무효표 일 경우. */
				FPlayerVoteData* Pair = CachedVoteEventsMap.Find(TEXT("NullVote"));
				if (Pair)
				{
					Pair->VotedCount++;
					CachedAlreadyVotersSet.Emplace(VotorAccountId);
				}
				else
				{
					FPlayerVoteData VoteData;
					VoteData.Candidate = nullptr;
					VoteData.VotedCount = 1;

					CachedAlreadyVotersSet.Emplace(VotorAccountId);
					CachedVoteEventsMap.Emplace(TEXT("NullVote"), VoteData);
				}
			}

			VotorRoleComponent->ResponseVoteEvent(InCandidate, EMafiaVoteFlag::Succeed);
		}
		else
		{
			VotorRoleComponent->ResponseVoteEvent(InCandidate, EMafiaVoteFlag::ImpossibleVote);
			return;
		}
	}
}

void UMafiaChairManManager::EndVote()
{
	for (auto& Pair : JoinedPlayerRoleComponents)
	{
		Pair.Value->FinishVoteEvent();
	}	

	CachedAbilityEventsHeap.Empty();
	CachedVoteEventsMap.Empty();
	CachedAlreadyVotersSet.Empty();
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
	else if (EMafiaFlowState::BeforeDay == InFlowState)
	{
		EMafiaGameResult Result = CheckGameOver();
		NotifyGameResult(Result);
	}
	else if (EMafiaFlowState::Day == InFlowState)
	{
		DispatchAbilityEvents();
		FlushAbilityEvents();
	}
	else if (EMafiaFlowState::AfterDay == InFlowState)
	{
		CachedAbilityEventsHeap.Empty();
		CachedVoteEventsMap.Empty();
	}
	else if (EMafiaFlowState::BeforeVote == InFlowState)
	{
		StartVote();
	}
	else if (EMafiaFlowState::Vote == InFlowState)
	{
		
	}
	else if (EMafiaFlowState::AfterVote == InFlowState)
	{
		EndVote();

		EMafiaGameResult Result = CheckGameOver();
		NotifyGameResult(Result);
	}
	else if (EMafiaFlowState::BeforeNight == InFlowState)
	{
		
	}
	else if (EMafiaFlowState::Night == InFlowState)
	{
		
	}
	else if (EMafiaFlowState::AfterNight == InFlowState)
	{
		EMafiaGameResult Result = CheckGameOver();
		NotifyGameResult(Result);
	}
			
}
