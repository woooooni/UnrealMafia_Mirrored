// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Manager/MafiaChairManManager.h"
#include "MafiaCore/Framework/System/MafiaLogChannels.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameMode.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerSeat.h"
#include "Framework/GameModes/MafiaBaseGameState.h"
#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerState.h"
#include "Algo/RandomShuffle.h"
#include "Mafia.h"


UMafiaChairManManager::UMafiaChairManManager(const FObjectInitializer& ObjectInitializer)
{	
	CachedAbilityEventsHeap.Reserve(16);
	CachedAbilityEventsHeap.Heapify();
}

bool UMafiaChairManManager::StartGame()
{
	return AssignAllPlayersAbility() && MakePlayersSeat();
}

void UMafiaChairManManager::EndGame()
{
	PlayerSeats.Empty();
}



bool UMafiaChairManManager::AssignAllPlayersAbility()
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

bool UMafiaChairManManager::MakePlayersSeat()
{
	UWorld* World = GetWorld();
	PlayerSeats.Empty();

	if (IsValid(World))
	{
		if (AMafiaBaseGameState* GS = World->GetGameState<AMafiaBaseGameState>())
		{
			uint8 Count = 0;

			for (auto& Pair : GS->GetJoinedUserPlayerStateMap())
			{
				if (UMafiaBasePlayerSeat* PlayerSeat = NewObject<UMafiaBasePlayerSeat>())
				{
					if (PlayerSeat->Initialize(GPlayerColors[Count], Pair.Value.Get()) == false)
					{
						return false;
					}

					PlayerSeats.Emplace(EMafiaColor(Count), PlayerSeat);
				}
				else
				{
					return false;
				}
				++Count;
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

				Event.OriginRole = InOrigin->GetRoleComponent()->GetRoleType();
				Event.OriginPlayer = InOrigin->GetRoleComponent();
				Event.DestPlayer = InDestination->GetRoleComponent();

				if (Event.OriginPlayer.IsValid() == false || Event.DestPlayer.IsValid() == false)
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
					Event.OriginPlayer->ResponseUseAbility(InDestination->GetRoleComponent(), EMafiaUseAbilityFlag::AlreadyUseAbility);
					return;
				}
				else
				{
					CachedAbilityEventsHeap.HeapPush(Event);
					Event.OriginPlayer->ResponseUseAbility(InDestination->GetRoleComponent(), EMafiaUseAbilityFlag::Succeed);
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

#pragma region Cheat
void UMafiaChairManManager::CheatChangeRole(AMafiaBasePlayerState* InPlayerState, UMafiaBaseRoleComponent* InNewRoleComponent)
{
#if ENABLE_CHEAT
	const FString& AccountIdStr = InPlayerState->GetUniqueId().ToString();
	if (AccountIdStr.IsEmpty())
	{
		MAFIA_ULOG(LogMafiaChairMan, Warning, TEXT("AccountIdStr is empty"));
		return;
	}

	if (IsValid(InNewRoleComponent))
	{
		const FName AccountId = FName(*AccountIdStr);
		if (auto Pair = JoinedPlayerRoleComponents.Find(AccountId))
		{
			JoinedPlayerRoleComponents.Add(AccountId, InNewRoleComponent);
		}
		else
		{
			JoinedPlayerRoleComponents.Emplace(AccountId, InNewRoleComponent);
		}
	}
#endif
}

#pragma endregion Cheat

void UMafiaChairManManager::BroadCastAbilityEvents()
{
	/** ktw : 능력사용 결과 통지를 위한 복사. */
	while (!CachedAbilityEventsHeap.IsEmpty())
	{
		FUseAbilityEventData Event;
		CachedAbilityEventsHeap.HeapPop(Event);
		
		if (Event.OriginPlayer.IsValid() && Event.DestPlayer.IsValid())
		{
			Event.DestPlayer.Get()->AffectedAbilityByOther(Event.OriginRole, Event.OriginPlayer.Get());
		}
	}

	CachedAbilityEventsHeap.Empty();
}

void UMafiaChairManManager::FlushAbilityEvents() const
{
	TArray<TObjectPtr<UMafiaBaseRoleComponent>> PlayerComponentsArray;
	JoinedPlayerRoleComponents.GenerateValueArray(PlayerComponentsArray);

	PlayerComponentsArray.HeapSort();
	for (auto RoleComponent : PlayerComponentsArray)
	{
		RoleComponent->AffectedEventsFlush();
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

UMafiaBaseRoleComponent* UMafiaChairManManager::FindDeadMan()
{
	/** ktw : 투표로 처형할 사람을 찾습니다. */
	TArray<FPlayerVoteData> VoteArray;
	CachedVoteEventsMap.GenerateValueArray(VoteArray);

	/** 1. 가장 득표수가 많은 Data를 찾고, 무효표 수와 비교해 처형할 사람을 정한다. */
	if (VoteArray.Num() > 0)
	{
		/**
			2. 무효표 수 계산.
				- (전체 플레이어 수 - 투표한 사람 수 + 1)
				- 무효 데이터 수 한 개 빼야 함.
				- 투표를 안했을 경우, 무효표로 합산한다.
		*/
		
		VoteArray.Sort([](const FPlayerVoteData& Left, const FPlayerVoteData& Right)
		{
			return Left.VotedCount > Right.VotedCount;
		});


		FPlayerVoteData MaxCountVoteData;
		uint8 NullVoteCount = 0;
		uint8 Sum = 0;

		for (auto Event : VoteArray)
		{
			if (Event.Candidate.IsValid())
			{
				MaxCountVoteData = Event.VotedCount > MaxCountVoteData.VotedCount ? Event : MaxCountVoteData;
			}
			else
			{
				/** 무효표 카운트 */
				NullVoteCount += Event.VotedCount;
			}
			Sum += Event.VotedCount;
		}

		/* 2. 전체  플레이어 숫자와 투표수를 비교해서 차이만큼 무효표에 반영. */ 
		NullVoteCount += (JoinedPlayerRoleComponents.Num() - Sum);
		
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

EMafiaVoteResultFlag UMafiaChairManManager::NotifyDeadMan()
{
	UMafiaBaseRoleComponent* DeathRow = FindDeadMan();
	EMafiaVoteResultFlag Flag = IsValid(DeathRow) ? EMafiaVoteResultFlag::SomeoneDying : EMafiaVoteResultFlag::NoDeathPlayer;
	
	for (auto& Pair : JoinedPlayerRoleComponents)
	{
		Pair.Value->ReceiveVoteResult(DeathRow, Flag);
	}
	
	return Flag;
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
			OutSuffledArray.Add(GCitizenAssignRoleArray[CitizenIdx]);
			++CitizenIdx;
			break;
		case EMafiaTeam::Mafia:
			OutSuffledArray.Add(GMafiaAssignRoleArray[MafiaIdx]);
			++MafiaIdx;
			break;
		case EMafiaTeam::Neutral:
			OutSuffledArray.Add(EMafiaRole::Killer);
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

EMafiaGameResult UMafiaChairManManager::CheckGameResult() const
{
	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		if (AMafiaBaseGameState* GameState = World->GetGameState<AMafiaBaseGameState>())
		{
			int8 AliveCitizenCount = 0;
			int8 AliveMafiaCount = 0;
			int8 AliveNeutralCount = 0;

			for (auto& Pair : JoinedPlayerRoleComponents)
			{
				/** 
					ktw : 마피아팀 승리조건 체크. 
					1순위 : 마피아팀 -> 2순위 : 중립 -> 3순위 : 시민팀
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
						else if (TeamType == EMafiaTeam::Neutral)
						{
							++AliveNeutralCount;
						}
					}
				}
			}

			if (AliveMafiaCount >= AliveCitizenCount + AliveNeutralCount)
			{
				return EMafiaGameResult::MafiaWin;
			}
			else if (AliveMafiaCount == 0)
			{
				if (AliveNeutralCount > 0)
				{
					if (AliveCitizenCount == 0)
					{
						return EMafiaGameResult::NeutralWin;
					}
					else
					{
						return EMafiaGameResult::None;
					}
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
	if (InFlowState == EMafiaFlowState::None)
	{
		CachedAlreadyVotersSet.Empty();
		CachedAbilityEventsHeap.Empty();
		CachedVoteEventsMap.Empty();
	}
	else if (InFlowState == EMafiaFlowState::BeforeDay) 
	{
		
	}
	else if(InFlowState == EMafiaFlowState::Day)
	{

	}
	else if(InFlowState == EMafiaFlowState::AfterDay)
	{
		CachedAlreadyVotersSet.Empty();
		CachedAbilityEventsHeap.Empty();
		CachedVoteEventsMap.Empty();
	}
	else if(InFlowState == EMafiaFlowState::EndDay)
	{

	}
	else if(InFlowState == EMafiaFlowState::BeforeVote)
	{
		StartVote();
	}
	else if(InFlowState == EMafiaFlowState::Vote)
	{

	}
	else if(InFlowState == EMafiaFlowState::AfterVote)
	{
		EndVote();
		EMafiaVoteResultFlag VoteFlag = NotifyDeadMan();
		if (VoteFlag == EMafiaVoteResultFlag::SomeoneDying)
		{

		}

		else
		{

		}
	}
	else if(InFlowState == EMafiaFlowState::EndVote)
	{
		
	}
	else if(InFlowState == EMafiaFlowState::BeforeNight)
	{
		
	}
	else if(InFlowState == EMafiaFlowState::Night)
	{
		
	}
	else if(InFlowState == EMafiaFlowState::AfterNight)
	{
		BroadCastAbilityEvents();
	}
	else if(InFlowState == EMafiaFlowState::EndNight)
	{
		FlushAbilityEvents();
	}
}
