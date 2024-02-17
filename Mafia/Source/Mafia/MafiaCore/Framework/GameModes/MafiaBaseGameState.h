// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaBaseGameState.generated.h"

class AMafiaBasePlayerState;
/**
 * GameState의 Owner는 없음
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class MAFIA_API AMafiaBaseGameState : public AGameState
{
	GENERATED_BODY()

protected:
	AMafiaBaseGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnRep_ReplicatedHasBegunPlay() override;
	virtual void BeginPlay() override;
	virtual void PostNetInit() override;
	virtual void PostInitializeComponents() override;

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void OnRep_MatchState() override;

	virtual bool HasGameStarted() const;
	virtual bool HasGameEnded() const;

	// ~ Begin GameState Interface
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	// ~ Begin Custom
	virtual void HandleRoundIsAssigningAbility();
	virtual void HandleInProgressMafia();
	virtual void HandleWaitingPostRound();
	// ~ End Custom
	virtual void HandleMatchHasEnded() override;
	//virtual void HandleLeavingMap() override;
	// ~ End GameState Interface

public:
	void SetMafiaFlowState(const EMafiaFlowState InState, const float InPlayTime);
	FORCEINLINE EMafiaFlowState GetMafiaFlowState() const { return MafiaFlowState; }

	bool AddToPlayerStateMap(AMafiaBasePlayerState* InPlayerState);
	const TMap<FName, TWeakObjectPtr<AMafiaBasePlayerState>>& GetJoinedUserPlayerStateMap() const { return JoinedUserPlayerStateMap; }

	FORCEINLINE int32 GetJoinedUserCount() const { return JoinedUserPlayerStateMap.Num(); }
	int32 GetReadyUserCount() const;

#pragma region Time
public:
	/** 서버기준의 실시간 월드 시간 */
	FORCEINLINE float GetReplicatedWorldTimeSeconds() const { return ReplicatedWorldTimeSecondsDouble; }
	FORCEINLINE void SetGameStartSeconds(const float InTime) { GameStartSeconds = InTime; }
	// #Todo-ksh : UpdateTimerStartSeconds와 SetTotalTimerSeconds룰 헙쳐도 될 수 있음
	FORCEINLINE void UpdateTimerStartSeconds() { TimerStartSeconds = ReplicatedWorldTimeSecondsDouble; }
	FORCEINLINE void SetTotalTimerSeconds(const float InTime) { TotalTimerSeconds = InTime; }
	FORCEINLINE float GetTotalTimerSeconds() { return TotalTimerSeconds; }

	/** 전체 게임 진행 시간 */
	virtual float GetGameElapsedTime() const;
	/** 전체 게임 진행 시간 */
	virtual float GetGameFlowTimerElapsedTime() const;
	/** 현재 게임 플로우 타이머의 남은 시간 */
	virtual float GetGameFlowTimerRemaining() const;
#pragma endregion Time

#pragma region Cheat
public:
	void AddGameDuration(const float InAddedSeconds) const;
#pragma endregion Cheat

private:
	UFUNCTION()
	void OnRep_MafiaFlowState();

private:
	/** 모든 유저가 Ready한 이후 게임 시작 시간. */
	UPROPERTY(Transient, Replicated)
	float GameStartSeconds;
	/** 게임 플로우 타이머의 시작 시간 */
	UPROPERTY(Transient, Replicated)
	float TimerStartSeconds;
	/** 현재 타이머 전체 진행 시간 */
	UPROPERTY(Transient, Replicated)
	float TotalTimerSeconds;

protected:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MafiaFlowState)
	EMafiaFlowState MafiaFlowState;
	EMafiaFlowState PrevFlowState;

	/** Key:AccoutId */
	TMap<FName, TWeakObjectPtr<AMafiaBasePlayerState>> JoinedUserPlayerStateMap;
};
