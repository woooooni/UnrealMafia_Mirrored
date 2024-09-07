// Copyright Epic Games, Inc. All Rights Reserved.

#include "MafiaCore/Framework/GameModes/MafiaBaseGameMode.h"
#include "MafiaCore/Framework/System/MafiaBaseGameInstance.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameSession.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerController.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "Framework/Manager/MafiaChairmanManager.h"
#include "MafiaCore/Framework/UI/Core/MafiaBaseHUD.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "Mafia.h"

AMafiaBaseGameMode::AMafiaBaseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("'/Game/StarterContent/Characters/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameSessionClass = AMafiaBaseGameSession::StaticClass();

	GameStartMinPlayerCount = 8;
	CurrentNumRound = 0;

	InProgressTime = 5.f;
	AssigningAbilityTime = 30.f;
	InitialVoteWaitTime = 30.f;
	NightTime = 30.f;
	VoteTime = 30.f;
	DayTime = 60.f;
}

#pragma optimize("", off)
void AMafiaBaseGameMode::PreInitializeComponents()
{
	/** GameState 생성, World->SetGameState(GameState), 월드에 GameState 등록 */
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin"));
	Super::PreInitializeComponents();
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));
}

void AMafiaBaseGameMode::InitGameState()
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin"));
	Super::InitGameState();
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));
}

void AMafiaBaseGameMode::BeginPlay()
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin : %s"), LOG_NETMODEINFO);
	Super::BeginPlay();
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));
}

void AMafiaBaseGameMode::Tick(float DeltaSeconds)
{
	// Super 호출 안하고 이곳에서 모두 처리
	
	if (GetMatchState() == MatchState::WaitingToStart)
	{
		// Check to see if we should start the match
		if (ReadyToStartMatch())
		{
			MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("GameMode returned ReadyToStartMatch"));
			StartMatch();
		}
	}

	// 모두 준비한 이후 InProgress에서 직업 선택전까지 연출
	if (GetMatchState() == MatchState::InProgress)
	{
		// 내부에서 Pending State를 통해 InProgress를 유지함
		SetMatchState(MafiaMatchState::AssigningAbility);
	}

	if (GetMatchState() == MafiaMatchState::AssigningAbility)
	{
		//  #Todo-ksh : 그 사이에 나갔다면 사실상 계속 진행해야함.
		if (NumPlayers == 0)
		{
			//SetMatchState(MatchState::WaitingToStart);
			ensure(false);
		}
		
		SetMatchState(MafiaMatchState::InProgressMafia);
	}

	/** 밑으로 가면 라운드 종료됨 */
	if (GetMatchState() == MafiaMatchState::InProgressMafia)
	{
		if (ReadyToEndRound())
		{
			SetMatchState(MafiaMatchState::WaitingPostRound);
		}
	}

	if (GetMatchState() == MafiaMatchState::WaitingPostRound)
	{
		// Check to see if we should end the match
		if (ReadyToEndMatch())
		{
			MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("GameMode returned ReadyToEndMatch"));
			EndMatch();
		}
	}
}

#pragma region AGameMode Interface
bool AMafiaBaseGameMode::ReadyToStartMatch_Implementation()
{
	// Super 사용하지 않음
	
	// for after second rounds
	if ((CurrentNumRound == 0 && IsReadyAllUsers())/* ||
		true == ReadyToAfterSecondRound()*/)
	{
		return true;
	}

	return false;
}

bool AMafiaBaseGameMode::ReadyToEndMatch_Implementation()
{
	return NumPlayers == 0;
}

bool AMafiaBaseGameMode::ReadyToEndRound()
{
	// 일단은 사람이 없다고 바로 종료되지 않도록 막아둠.
	return false;
}

bool AMafiaBaseGameMode::IsMatchInProgress() const
{
	if (MatchState == MafiaMatchState::AssigningAbility ||
	MatchState == MafiaMatchState::InProgressMafia ||
	MatchState == MafiaMatchState::WaitingPostRound)
	{
		MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("IsMatchInProgress : True"));
		return true;
	}
	
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("%s"), Super::IsMatchInProgress() ? TEXT("True") : TEXT("False"));
	return Super::IsMatchInProgress();
}

void AMafiaBaseGameMode::StartMatch()
{
	Super::StartMatch();
}

void AMafiaBaseGameMode::EndMatch()
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin"));
	Super::EndMatch();
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));
}

void AMafiaBaseGameMode::RestartGame()
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin"));
	Super::RestartGame();
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));
}

void AMafiaBaseGameMode::HandleMatchIsWaitingToStart()
{
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("Begin"));
	Super::HandleMatchIsWaitingToStart();
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("End"));
}

void AMafiaBaseGameMode::HandleMatchHasStarted()
{
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("Begin"));
	Super::HandleMatchHasStarted();
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("End"));
}

void AMafiaBaseGameMode::HandleRoundIsAssigningAbility()
{
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("HandleRoundIsAssigningAbility"));

	AMafiaBaseGameSession* MafiaBaseGameSession = Cast<AMafiaBaseGameSession>(GameSession);
	
	if (IsValid(MafiaBaseGameSession))
	{
		MafiaBaseGameSession->HandleRoundIsAssigningAbility();
	}
	else
	{
		ensure(false);
	}

}

void AMafiaBaseGameMode::HandleInProgressMafia()
{
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("HandleInProgressMafia"));
	AMafiaBaseGameState* MafiaBaseGameState = GetGameState<AMafiaBaseGameState>();
	
	if (IsValid(MafiaBaseGameState) == false)
	{
		ensure(false);
		return;
	}


	UWorld* MyWorld = GetWorld();
	if (IsValid(MyWorld) == false)
	{
		ensure(false);
		return;
	}

	const EMafiaFlowState CurrentFlowState = MafiaBaseGameState->GetMafiaFlowState();
	if (CurrentFlowState == EMafiaFlowState::Vote)
	{
		AMafiaBaseGameSession* MafiaBaseGameSession = Cast<AMafiaBaseGameSession>(GameSession);
		if (IsValid(MafiaBaseGameSession))
		{
			MafiaBaseGameSession->HandleInProgressMafia();
		}
		else
		{
			ensure(false);
		}
	}

	UMafiaChairmanManager* ChairMan = GetChairMan();
	if (IsValid(ChairMan) == false)
	{
		ensure(false);
		return;
	}
	
	// 최초 투표만 예외
	
	/** #Todo - ktw : 각 FlowState의 연출시간까지 고려한 WaitTime 설정. */
	if (CurrentFlowState == EMafiaFlowState::None)
	{
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::None, 3.f);
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::BeforeDay, InitialVoteWaitTime);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, InitialVoteWaitTime);
	}
	else if (CurrentFlowState == EMafiaFlowState::BeforeDay)
	{	
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::Day, DayTime);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, DayTime);
	}		
	else if (CurrentFlowState == EMafiaFlowState::Day)
	{		 
		//if (MyWorld->GetTimerManager().IsTimerActive(MainGameFlowTimerHandle))
		//{
		//	MyWorld->GetTimerManager().ClearTimer(MainGameFlowTimerHandle);
		//}

		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::AfterDay, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}		 
	else if (CurrentFlowState == EMafiaFlowState::AfterDay)
	{	
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::EndDay, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}		
	else if (CurrentFlowState == EMafiaFlowState::EndDay)
	{
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::BeforeVote, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}
	else if (CurrentFlowState == EMafiaFlowState::BeforeVote)
	{	
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::Vote, VoteTime);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, VoteTime);
	}		 
	else if (CurrentFlowState == EMafiaFlowState::Vote)
	{		 
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::AfterVote, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}		 
	else if (CurrentFlowState == EMafiaFlowState::AfterVote)
	{		 
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::EndVote, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}		 
	else if (CurrentFlowState == EMafiaFlowState::EndVote)
	{
		AMafiaBasePlayerState* DeadMan = ChairMan->FindDeadMan();
		if (IsValid(DeadMan))
		{
			MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::BeginPunishment, 10.f);
			MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 10.f);
		}
		else
		{
			MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::BeforeNight, 10.f);
			MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 10.f);
		}
		ChairMan->NotifyDeadMan(DeadMan);
	}
	else if (CurrentFlowState == EMafiaFlowState::BeginPunishment)
	{
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::Punishment, 10.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 10.f);
	}
	else if (CurrentFlowState == EMafiaFlowState::Punishment)
	{
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::AfterPunishment, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}
	else if (CurrentFlowState == EMafiaFlowState::AfterPunishment)
	{
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::EndPunishment, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}
	else if (CurrentFlowState == EMafiaFlowState::EndPunishment)
	{
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::BeforeNight, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}
	else if (CurrentFlowState == EMafiaFlowState::BeforeNight)
	{		 
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::Night, NightTime);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, NightTime);
	}		 
	else if (CurrentFlowState == EMafiaFlowState::Night)
	{		 
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::AfterNight, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}		 
	else if (CurrentFlowState == EMafiaFlowState::AfterNight)
	{
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::EndNight, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}
	else if (CurrentFlowState == EMafiaFlowState::EndNight)
	{
		MafiaBaseGameState->SetMafiaFlowState(EMafiaFlowState::BeforeDay, 1.f);
		MyWorld->GetTimerManager().SetTimer(MainGameFlowTimerHandle, this, &AMafiaBaseGameMode::HandleInProgressMafia, 1.f);
	}

}

void AMafiaBaseGameMode::HandleWaitingPostRound()
{
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("HandleWaitingPostRound"));

	AMafiaBaseGameSession* MafiaBaseGameSession = Cast<AMafiaBaseGameSession>(GameSession);
	if (IsValid(MafiaBaseGameSession))
	{
		MafiaBaseGameSession->HandleWaitingPostRound();
	}
	else
	{
		ensure(false);
	}
}

void AMafiaBaseGameMode::HandleMatchHasEnded()
{
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("Begin"));
	Super::HandleMatchHasEnded();
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("End"));
}
#pragma endregion AGameMode Interface

#pragma region AGameModeBase Interface
void AMafiaBaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	/** @See void UWorld::InitializeActorsForPlay(const FURL& InURL, bool bResetTime, FRegisterComponentContext* Context) 에서 호출 */
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("Begin"));
	Super::InitGame(MapName, Options, ErrorMessage);
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("End"));
}

void AMafiaBaseGameMode::StartPlay()
{
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("Begin"));
	Super::StartPlay();
	MAFIA_ALOG(LogMafiaGameMode, Warning, TEXT("End"));
}

bool AMafiaBaseGameMode::HasMatchStarted() const
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Called"));
	return Super::HasMatchStarted();
}

bool AMafiaBaseGameMode::HasMatchEnded() const
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Called"));
	return Super::HasMatchEnded();
}

void AMafiaBaseGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("============================================================================="));
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));
}

APlayerController* AMafiaBaseGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Called"));
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void AMafiaBaseGameMode::PostLogin(APlayerController* NewPlayer)
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin"));
	Super::PostLogin(NewPlayer);
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));

	if (false == IsValid(NewPlayer))
	{
		return;
	}

	AMafiaBasePlayerState* NewPlayerState = Cast<AMafiaBasePlayerState>(NewPlayer->PlayerState);
	if (false == IsValid(NewPlayerState))
	{
		return;
	}

	AMafiaBaseGameState* MafiaBaseGameState = GetGameState<AMafiaBaseGameState>();
	if (false == IsValid(MafiaBaseGameState))
	{
		return;
	}
}

void AMafiaBaseGameMode::Logout(AController* Exiting)
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin"));
	Super::Logout(Exiting);
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));
}

void AMafiaBaseGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Begin"));
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("End"));
}

bool AMafiaBaseGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Called"));
	return Super::PlayerCanRestart_Implementation(Player);
}

FString AMafiaBaseGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Called"));
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void AMafiaBaseGameMode::OnEndInProgressState()
{
	if (PendingMatchState == MafiaMatchState::AssigningAbility)
	{
		MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Ended InProgress"));
		SwitchMatchState(MafiaMatchState::AssigningAbility);
	}
	else
	{
		ensure(false);
	}
}

void AMafiaBaseGameMode::OnEndAssigningAbilityState()
{
	/** 여기 들어왔다는건, 이전까지의 모든 직업 선택 및 연출이 종료되었고, 실제 마피아 게임이 시작됨을 의미 */

	/**
	 * 이전까지 완료되었어야 하는 플로우
	 * - FadeInOut으로 밤에서 낮으로 변화는 연출
	 * - (Optional) 카메라의 이동 연출
	 * - 카메라 전환 (쿼터뷰)
	 * - 채팅 기능 활성화 (이건 좀 고민해봐야할듯. 처음부터 열어두는게 자연스러워 보임)
	 * - 여기까지의 내용을 모두 처리해야함
	 */

	if (PendingMatchState == MafiaMatchState::InProgressMafia)
	{
		MAFIA_ALOG(LogMafiaGameMode, Log, TEXT("Ended AssignAbility"));
		SwitchMatchState(MafiaMatchState::InProgressMafia);
		
	}
	else
	{
		ensure(false);
	}
}

#pragma endregion AGameModeBase Interface

void AMafiaBaseGameMode::SetMatchState(FName NewState)
{
	if (MatchState == NewState || PendingMatchState == NewState)
	{
		return;
	}

	const bool bPendingSwitch = OnPendingMatchStateSetForInProgress(NewState);
	if (bPendingSwitch)
	{
		PendingMatchState = NewState;
	}
	else
	{
		SwitchMatchState(NewState);
	}
}

void AMafiaBaseGameMode::OnMatchStateSet()
{
	// Super 호출해서는 안됨
	FGameModeEvents::OnGameModeMatchStateSetEvent().Broadcast(MatchState);

	if (MatchState == MatchState::WaitingToStart)
	{
		HandleMatchIsWaitingToStart();
	}
	else if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MafiaMatchState::AssigningAbility)
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
	else if (MatchState == MatchState::WaitingPostMatch)
	{
		HandleMatchHasEnded();
	}
	else if (MatchState == MatchState::LeavingMap)
	{
		HandleLeavingMap();
	}
	else if (MatchState == MatchState::Aborted)
	{
		HandleMatchAborted();
	}
}

bool AMafiaBaseGameMode::OnPendingMatchStateSetForInProgress(FName NewPendingState)
{
	/** 이곳에서 Pending에 대한 처리 */

	//if (NewPendingState == MafiaMatchState::InProgress)
	//{
	//	SetEstimatedTimeForRoundStartSeconds();
	//}
	//else if (NewPendingState == MatchState::InProgress)
	//{
	//	SetEstimatedTimeForSessionStartSeconds();
	//	SetEstimatedTimeForRoundStartSeconds();
	//}

	//if (true == FMath::IsNearlyZero(TimerTimeForStartingGame))
	//{
	//	TimerTimeForStartingGame = 0.1f;
	//}

	//// OnPendingMatchStateSet 함수가 호출됐을 때 InProgress 상태라면 Timer Clear
	if (PendingMatchState == MafiaMatchState::InProgressMafia)
	{
		ensure(false);
		//GetWorldTimerManager().ClearTimer(AssignAbilityTimerHandle);
	}
	else if (NewPendingState == MafiaMatchState::AssigningAbility)
	{
		/** 직업 선택 이전까지의 연출 플로우 시작 부분 - InProgress 상태 */
		if (AMafiaBaseGameState* MafiaBaseGameState = GetGameState<AMafiaBaseGameState>())
		{
			MafiaBaseGameState->SetTotalTimerSeconds(InProgressTime);
			MafiaBaseGameState->UpdateTimerStartSeconds();
			GetWorld()->GetTimerManager().SetTimer(InProgressTimeTimerHandle, this, &AMafiaBaseGameMode::OnEndInProgressState, InProgressTime);
			return true;
		}
	}
	else if (NewPendingState == MafiaMatchState::InProgressMafia)
	{
		/** 직업 선택에 대한 플로우 시작 부분 - AssigningAbility 상태 */
		if (AMafiaBaseGameState* MafiaBaseGameState = GetGameState<AMafiaBaseGameState>())
		{
			MafiaBaseGameState->SetTotalTimerSeconds(AssigningAbilityTime);
			MafiaBaseGameState->UpdateTimerStartSeconds();
			GetWorld()->GetTimerManager().SetTimer(AssignAbilityTimerHandle, this, &AMafiaBaseGameMode::OnEndAssigningAbilityState, AssigningAbilityTime);

			if (UMafiaChairmanManager* ChairMan = GetChairMan())
			{
				bool Succeed = ChairMan->StartGame();
				if (Succeed == false)
				{
					check(Succeed);
				}
			}
			return true;
		}
	}

	return false;
}

void AMafiaBaseGameMode::SwitchMatchState(FName NewState)
{
	FName NextMatchState;
	if (PendingMatchState.IsNone())
	{
		NextMatchState = NewState;
	}
	else
	{
		NextMatchState = PendingMatchState;
		PendingMatchState = NAME_None;
	}

	Super::SetMatchState(NextMatchState);
}

bool AMafiaBaseGameMode::IsReadyAllUsers() const
{
	AMafiaBaseGameState* MafiaBaseGameState = GetGameState<AMafiaBaseGameState>();
	if (false == IsValid(MafiaBaseGameState))
	{
		return false;
	}

	const TMap<FName, TWeakObjectPtr<AMafiaBasePlayerState>>& JoinedUserPlayerStateMap = MafiaBaseGameState->GetJoinedUserPlayerStateMap();
	if (JoinedUserPlayerStateMap.Num() < GameStartMinPlayerCount)
	{
		return false;
	}

	int32 UserReadyCount = 0;
	for (const auto& Elem : JoinedUserPlayerStateMap)
	{
		const AMafiaBasePlayerState* const UserPlayerState = Elem.Value.Get();
		if (false == IsValid(UserPlayerState))
		{
			continue;
		}

		if (UserPlayerState->IsReadyForGame())
		{
			UserReadyCount++;
		}
	}

	if (UserReadyCount < GameStartMinPlayerCount)
	{
		return false;
	}

	return true;
}

void AMafiaBaseGameMode::AddGameDuration(const float InAddedSeconds)
{
	// #TODO-ksh : 타이머에 대한 치트키 추가해주기

	if (false == IsMatchInProgress())
	{
		return;
	}

	float AdjustedTime = InAddedSeconds;
	if (PendingMatchState == MafiaMatchState::AssigningAbility)
	{
		//if (AssignAbilityTimerHandle.IsValid() && GetWorldTimerManager().IsTimerActive(AssignAbilityTimerHandle))
		//{
		//	const float RemainingTime = GetWorldTimerManager().GetTimerRemaining(AssignAbilityTimerHandle);
		//	const float NewTime = FMath::Max(RemainingTime + AdjustedTime, 0.1f); // 0.1초 정도는 있어야함. 0초면 타이머가 제대로 동작하지 않기 때문
		//	
		//	if (RemainingTime + AdjustedTime < 0.f)
		//	{
		//		AdjustedTime = 0.1f - RemainingTime;
		//	}

		//	GetWorldTimerManager().ClearTimer(AssignAbilityTimerHandle);
		//	GetWorldTimerManager().SetTimer(AssignAbilityTimerHandle, this, &AMafiaBaseGameMode::OnAssignAbility, NewTime, false);
		//	
		//	if (AMafiaBaseGameState* MafiaBaseGameState = GetGameState<AMafiaBaseGameState>())
		//	{
		//		MafiaBaseGameState->SetTotalTimerSeconds(AssigningAbilityTime + AdjustedTime);
		//	}
		//}
	}
	else
	{

	}
}

UMafiaChairmanManager* AMafiaBaseGameMode::GetChairMan()
{
	if (UMafiaBaseGameInstance* GI = GetGameInstance<UMafiaBaseGameInstance>())
	{
		return GI->GetChairMan();
	}
	return nullptr;
}

UE_NODISCARD
bool AMafiaBaseGameMode::CheckGameOver()
{
	if (UMafiaChairmanManager* ChairMan = GetChairMan())
	{
		return EMafiaGameResult::None != ChairMan->CheckGameResult();
	}
	return true;
}

#pragma optimize("", on)