// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MafiaBaseGameMode.generated.h"

class AMafiaBasePlayerState;

UCLASS(Abstract)
class AMafiaBaseGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	AMafiaBaseGameMode();

	/** GameState 생성 */
	virtual void PreInitializeComponents() override;
	virtual void InitGameState() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	//~ Begin AGameMode Interface
	virtual bool ReadyToStartMatch_Implementation() override;
	//virtual bool ReadyToAfterSecondRound();
	//virtual bool ReadyToStartRound();
	virtual bool ReadyToEndRound();
	virtual bool ReadyToEndMatch_Implementation() override;
	virtual bool IsMatchInProgress() const override;

	virtual void StartMatch() override;
	virtual void EndMatch() override;
	virtual void RestartGame() override;

	/** StartPlay() 안에서 SetMatchState(MatchState::WaitingToStart) 된후 호출됨  */
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
// Added Custom
	virtual void HandleRoundIsAssigningAbility();
	virtual void HandleInProgressMafia();
	virtual void HandleWaitingPostRound();
// Added Custom
	virtual void HandleMatchHasEnded() override;
	//~ End AGameMode Interface
	
	//~ Begin AGameModeBase Interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual bool HasMatchStarted() const override;
	virtual bool HasMatchEnded() const override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
	//~ End AGameModeBase Interface

	// #TODO-ksh : 필요에 따라 추가하기
	//virtual void OnSpawnPlayerCharacter(AController* NewPlayer, APawn* NewPawn);

protected:
	/** InProgress에서 AssigningAbility로 바뀌기 전까지의 Pending 타이머가 종료된 후 호출되는 함수 */
	UFUNCTION()
	virtual void OnEndInProgressState();
	/** AssigningAbility에서 InProgressMafia로 바뀌기 전까지의 Pending 타이머가 종료된 후 호출되는 함수 */
	UFUNCTION()
	virtual void OnEndAssigningAbilityState();

protected:
	virtual void SetMatchState(FName NewState) final;
	virtual void OnMatchStateSet() final;

	virtual bool OnPendingMatchStateSetForInProgress(FName NewPendingState);
	virtual void SwitchMatchState(FName NewState);

	virtual bool IsReadyAllUsers() const;

#pragma region Cheat
public:
	virtual void AddGameDuration(const float InAddedSeconds);
	FORCEINLINE void SetGameStartMinPlayerCount(const int32 InPlayerCount) { GameStartMinPlayerCount = InPlayerCount; }
#pragma endregion

protected:
	class UMafiaChairmanManager* GetChairMan();
	bool CheckGameOver();

protected:
	/** 게임을 시작하기 위한 최소 인원 */
	UPROPERTY(EditAnywhere, Category = "GameRule", meta = (ClampMin = "1", ClampMax = "12"))
	int32 GameStartMinPlayerCount;
	/** 게임 시작후 연출 */
	UPROPERTY(EditAnywhere, Category = "Timer", meta = (ClampMin = "5", ClampMax = "60"))
	float InProgressTime;
	/** 연출 이후 능력 부여 + 시작 연출 타이머 시간 */
	UPROPERTY(EditAnywhere, Category = "Timer", meta = (ClampMin = "5", ClampMax = "60"))
	float AssigningAbilityTime;

protected:
	/** 최초 투표까지의 시간 */
	UPROPERTY(EditAnywhere, Category = "Timer", meta = (ClampMin = "5", ClampMax = "60"))
	float InitialVoteWaitTime;
	/** 공통 낮 시간 */
	UPROPERTY(EditAnywhere, Category = "Timer", meta = (ClampMin = "5", ClampMax = "60"))
	float DayTime;

	/** 공통 투표 시간 */
	UPROPERTY(EditAnywhere, Category = "Timer", meta = (ClampMin = "5", ClampMax = "60"))
	float VoteTime;

	/** 공통 밤 시간 */
	UPROPERTY(EditAnywhere, Category = "Timer", meta = (ClampMin = "5", ClampMax = "60"))
	float NightTime;

	int32 CurrentNumRound;
	FName PendingMatchState;

protected:
	FTimerHandle InProgressTimeTimerHandle;
	FTimerHandle AssignAbilityTimerHandle;

	FTimerHandle MainGameFlowTimerHandle;
};