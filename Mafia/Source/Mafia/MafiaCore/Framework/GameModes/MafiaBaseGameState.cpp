// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameMode.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/System/MafiaGameEvents.h"
#include "MafiaCore/Framework/System/MafiaBaseGameInstance.h"
#include "Framework/Manager/MafiaChairmanManager.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "Mafia.h"

#include "Net/UnrealNetwork.h"

AMafiaBaseGameState::AMafiaBaseGameState()
{
	GameStartSeconds = 0.f;
	TimerStartSeconds = 0.f;
	TotalTimerSeconds = 0.f;
	GameRound = 0;
	MafiaFlowState = EMafiaFlowState::None;
	PrevFlowState = EMafiaFlowState::None;
}

void AMafiaBaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMafiaBaseGameState, GameStartSeconds);
	DOREPLIFETIME(AMafiaBaseGameState, TimerStartSeconds);
	DOREPLIFETIME(AMafiaBaseGameState, TotalTimerSeconds);
	DOREPLIFETIME(AMafiaBaseGameState, MafiaFlowState);
	DOREPLIFETIME(AMafiaBaseGameState, GameRound);
	
}

void AMafiaBaseGameState::PostNetInit()
{
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("Begin"));
	Super::PostNetInit();
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("End"));
}

void AMafiaBaseGameState::PostInitializeComponents()
{
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("Begin"));

	/** 여기서 최초로 GetWorld()에 대한 GameState가 등록된다 */
	Super::PostInitializeComponents();
	
	if (PlayerArray.IsEmpty())
	{
		MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("PlayerArray is empty"));
	}
	else
	{
		MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("PlayerArray:%d"), PlayerArray.Num());
	}

	/** 오직 클라에서 추가하는 용도. 항상 여기서 추가되지는 않음. 그리고 데디서버는 여기서 추가되지 못함. @See AMafiaBasePlayerState::OnSetUniqueId() */
	for (APlayerState* const PlayerState : PlayerArray)
	{
		AddToPlayerStateMap(Cast<AMafiaBasePlayerState>(PlayerState));
	}

	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("End"));
}

void AMafiaBaseGameState::AddPlayerState(APlayerState* PlayerState)
{
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("Begin. PlayerArray:%d"), PlayerArray.Num());
	Super::AddPlayerState(PlayerState);
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("End. PlayerArray:%d"), PlayerArray.Num());
}

void AMafiaBaseGameState::BeginPlay()
{
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("Begin"));
	Super::BeginPlay();
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("End"));
}

void AMafiaBaseGameState::OnRep_ReplicatedHasBegunPlay()
{
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("Begin"));
	Super::OnRep_ReplicatedHasBegunPlay(); // 모든 월드내 액터 BeginPlay 호출
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("End"));
}

void AMafiaBaseGameState::OnRep_MatchState()
{
	/** 이곳은 데디서버가 아닌 AutonomousProxy & SimulatedProxy 모두가 들어올 수 있습니다 */
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("Begin, PrevMatchState:%s, MatchState:%s"), *PreviousMatchState.ToString(), *MatchState.ToString());
	Super::OnRep_MatchState();

	// Custom Mafia Round State
	if (MatchState == MafiaMatchState::AssigningAbility)
	{
		HandleRoundIsAssigningAbility();
	}
	else if (MatchState == MafiaMatchState::InProgressMafia)
	{
		HandleInProgressMafia();
	}
	else if (MatchState == MafiaMatchState::WaitingPostRound)
	{
		HandleWaitingPostRound();
	}

	SendGameEvent_OneParam(OnChangedMatchState, MatchState);
	
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("End"));
}

bool AMafiaBaseGameState::HasGameStarted() const
{
	if (false == HasMatchStarted())
	{
		return false;
	}

	return true;
}

bool AMafiaBaseGameState::HasGameEnded() const
{
	if (GetMatchState() == MafiaMatchState::WaitingPostRound || GetMatchState() == MatchState::WaitingPostMatch)
	{
		return true;
	}

	return false;
}

void AMafiaBaseGameState::HandleMatchIsWaitingToStart()
{
	/**
	 * 모든 유저가 Ready를 누르기를 기다리는 상태. 게임이 시작되면 여기까지 다이렉트로 진행됨.
	 */
	Super::HandleMatchIsWaitingToStart();
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("HandleMatchIsWaitingToStart"));
}

void AMafiaBaseGameState::HandleMatchHasStarted()
{
	/**
	 * 직업 선택하기 전까지 대기 시간
	 */
	Super::HandleMatchHasStarted();
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("HandleMatchHasStarted"));
}

void AMafiaBaseGameState::HandleRoundIsAssigningAbility()
{
	/**
	 * 게임 시작후, 연출 이후 직업 선택. 그리고 연출까지 포함
	 */
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("HandleRoundIsAssigningAbility"));
}

void AMafiaBaseGameState::HandleInProgressMafia()
{
	/**
	 * 직업선택(AssigningAbility)까지 모두 끝난뒤, 진입 함수
	 */
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("HandleRoundHasStarted"));
}

void AMafiaBaseGameState::HandleWaitingPostRound()
{
	/**
	 * 마피아 게임이 끝나고 결과가 나오는 진입 함수
	 */
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("HandleRoundHasEnded"));
}

void AMafiaBaseGameState::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	MAFIA_ALOG(LogMafiaGameState, Log, TEXT("HandleMatchHasEnded"));
}

bool AMafiaBaseGameState::AddToPlayerStateMap(AMafiaBasePlayerState* InPlayerState)
{
	if (false == IsValid(InPlayerState))
	{
		MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("InPlayerState is invalid"));
		return false;
	}

	const FString& AccountIdStr = InPlayerState->GetUniqueId().ToString();
	if (AccountIdStr.IsEmpty())
	{
		MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("AccountIdStr is empty"));
		return false;
	}

	const FName AccountId = FName(*AccountIdStr);
	if (JoinedUserPlayerStateMap.Find(AccountId))
	{
		MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("Already exist AccountId:%s"), *AccountId.ToString());
		return false;
	}
	else
	{
		JoinedUserPlayerStateMap.Emplace(AccountId, InPlayerState);
		MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("Success Added AccountId:%s"), *AccountId.ToString());
		return true;
	}
}

void AMafiaBaseGameState::SetMafiaFlowState(const EMafiaFlowState InState, const float InPlayTime)
{
	SetTotalTimerSeconds(InPlayTime);
	UpdateTimerStartSeconds();

	MafiaFlowState = InState;
	if (UMafiaBaseGameInstance* GameInstance = Cast<UMafiaBaseGameInstance>(GetGameInstance()))
	{
		if (GameInstance->IsDedicatedServerInstance())
		{
			if (UMafiaChairmanManager* ChairMan = GameInstance->GetChairMan())
			{
				ChairMan->OnSetMafiaFlowState(MafiaFlowState);
			}
		}
	}

	if (MafiaFlowState == EMafiaFlowState::BeforeDay)
	{
		GameRound++;
	}

	OnRep_MafiaFlowState(); // server call
}

float AMafiaBaseGameState::GetGameElapsedTime() const
{
	if (false == HasGameStarted())
	{
		return 0.f;
	}

	return FMath::Max(0.f, ReplicatedWorldTimeSecondsDouble - GameStartSeconds);
}

float AMafiaBaseGameState::GetGameFlowTimerElapsedTime() const
{
	if (false == HasGameStarted())
	{
		return 0.f;
	}

	return FMath::Max(0.f, ReplicatedWorldTimeSecondsDouble - TimerStartSeconds);
}

float AMafiaBaseGameState::GetGameFlowTimerRemaining() const
{
	if (false == HasGameStarted())
	{
		return 0.f;
	}

	const float GameFlowElapsedTime = GetGameFlowTimerElapsedTime();
	if (GameFlowElapsedTime < 0 || FMath::IsNearlyZero(GameFlowElapsedTime))
	{
		return 0.f;
	}

	return FMath::Max(0.f, TotalTimerSeconds - GameFlowElapsedTime);
}

int32 AMafiaBaseGameState::GetReadyUserCount() const
{
	int32 ReadyUserCount = 0;
	for (const auto& Elem : JoinedUserPlayerStateMap)
	{
		if (AMafiaBasePlayerState* PC = Elem.Value.Get())
		{
			if (PC->IsReadyForGame())
			{
				ReadyUserCount++;
			}
		}
	}

	return ReadyUserCount;
}

void AMafiaBaseGameState::AddGameDuration(const float InAddedSeconds) const
{
	if (false == IsMatchInProgress() || false == HasGameStarted())
	{
		return;
	}

	AMafiaBaseGameMode* MafiaBaseGameMode = GetWorld()->GetAuthGameMode<AMafiaBaseGameMode>();
	if (true == IsValid(MafiaBaseGameMode))
	{
		MafiaBaseGameMode->AddGameDuration(InAddedSeconds);
	}
}


void AMafiaBaseGameState::OnRep_MafiaFlowState()
{
	MAFIA_ALOG(LogMafiaGameState, Warning, TEXT("from %s to %s, Timer:%f"), *EnumToStringMafia(EMafiaFlowState, PrevFlowState), *EnumToStringMafia(EMafiaFlowState, MafiaFlowState), GetTotalTimerSeconds());
	
	SendGameEvent_OneParam(OnChangedMafiaFlowState, MafiaFlowState);
	PrevFlowState = MafiaFlowState;
	
}