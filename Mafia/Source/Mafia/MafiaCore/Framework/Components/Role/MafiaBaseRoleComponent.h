// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Framework/Types/MafiaTypes.h"
#include "MafiaBaseRoleComponent.generated.h"

USTRUCT()
struct FBroadCastEvent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> EventSender;

	UPROPERTY()
	EMafiaBroadCastEvent EventType;

public:
	friend bool operator < (const FBroadCastEvent& Left, const FBroadCastEvent& Right)
	{
		return Left.EventType < Right.EventType;
	}
};


USTRUCT()
struct FAffectedEvent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> AbilityPlayer;
	EMafiaRole AbilityPlayerRole;

public:
	friend bool operator < (const FAffectedEvent& Left, const FAffectedEvent& Right)
	{
		return Left.AbilityPlayerRole > Right.AbilityPlayerRole;
	};
};


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MAFIA_API UMafiaBaseRoleComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UMafiaBaseRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnUnregister() override;

	UFUNCTION()
	virtual void BindDelegates();

	UFUNCTION()
	virtual void UnBindDelegates();

	UFUNCTION()
	virtual void ResetForNextRound();

#pragma region Getter & Setter
public:
	UFUNCTION()
	void SetTeamType(EMafiaTeam InTeam);
	FORCEINLINE EMafiaTeam GetTeamType() { return TeamType; }

	UFUNCTION()
	void SetRoleType(EMafiaRole InRole);
	FORCEINLINE EMafiaRole GetRoleType() { return RoleType; }

	UFUNCTION()
	void SetDead(bool InDead);
	FORCEINLINE bool IsDead() { return bDead; }

	UFUNCTION()
	void SetRoleName(FName InRoleName);
	FORCEINLINE FName GetRoleName() { return RoleName; }

	UFUNCTION()
	FORCEINLINE class AMafiaBasePlayerState* GetOwningPlayerState() const { return OwningPlayerState.Get(); }
#pragma endregion Getter & Setter

#pragma region Getter & Setter RPC
	
	UFUNCTION(Server, Reliable)
	void ServerReqSetTeamType(EMafiaTeam InTeam);

	UFUNCTION(Server, Reliable)
	void ServerReqSetRoleType(EMafiaRole InRole);
	
	UFUNCTION(Server, Reliable)
	void ServerReqSetDead(bool InDead);

	UFUNCTION(Server, Reliable)
	void ServerReqSetRoleName(FName InRoleName);
#pragma endregion Getter & Setter RPC

public:

	UFUNCTION()
	void SendBroadCastEvent(const EMafiaBroadCastEvent& InEvent);

	UFUNCTION()
	void ReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent);

	UFUNCTION()
	void Vote(class AMafiaBasePlayerState* InOther);
	

	

#pragma region Role Ability(역할 능력 관련)
public:

	UFUNCTION()
	virtual void UseAbility(class AMafiaBasePlayerState* InOther);

	/** 
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void AffectedAbilityByOther(EMafiaRole InRole, AMafiaBasePlayerState* InOther);

	/**
		ktw : 서버에서 호출해야 합니다.
	*/
	UFUNCTION()
	void ReceiveInstantEvent(AMafiaBasePlayerState* InOther);


	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void NotifyResultAbility(AMafiaBasePlayerState* InOther);
#pragma endregion Role Ability(역할 능력 관련)
	
#pragma region Vote(투표)
public:
	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void StartVoteEvent();


	UFUNCTION()
	void ReceiveVoteResult(AMafiaBasePlayerState* InDeadMan, EMafiaVoteResultFlag InFlag);

	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void FinishVoteEvent();
#pragma endregion Vote

#pragma region Role Ability RPC
protected:
	UFUNCTION(Server, Reliable)
	void ServerReqUseAbility(AMafiaBasePlayerState* InOther, EMafiaAbilityEventType InEventType);

	UFUNCTION(Server, Reliable)
	void ServerReqSendBroadCastEvent(const EMafiaBroadCastEvent InEvent);

protected:
	UFUNCTION(Client, Reliable)
	void ClientAffectedAbilityByOther(EMafiaRole InRole, AMafiaBasePlayerState* InOther);

	UFUNCTION(Client, Reliable)
	void ClientNotifyResultAbility(AMafiaBasePlayerState* InOther);

	UFUNCTION(Client, Reliable)
	void ClientResponseUseAbility(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType);

	UFUNCTION(Client, Reliable)
	void ClientAbilityEventsFlush();

	UFUNCTION(Client, Reliable)
	void ClientReceiveInstantEvent(AMafiaBasePlayerState* InOther);

	UFUNCTION(Client, Reliable)
	void ClientReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent InEvent);
#pragma endregion Role Ability RPC

#pragma region Vote(투표) RPC
protected:
	UFUNCTION(Server, Reliable)
	void ServerReqVote(AMafiaBasePlayerState* InOther);

	UFUNCTION(Client, Reliable)
	void ClientStartVoteEvent();

	UFUNCTION(Client, Reliable)
	void ClientResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag);

	UFUNCTION(Client, Reliable)
	void ClientReceiveVoteResult(AMafiaBasePlayerState* InDeadMan, EMafiaVoteResultFlag InFlag);

	UFUNCTION(Client, Reliable)
	void ClientFinishVoteEvent();
#pragma endregion Vote RPC


protected:
	/** ktw : Only Exec On Client */
	virtual void HandleResponseUseAbility(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType);
	virtual void HandleReceiveAffectedAbility(EMafiaRole InRole, AMafiaBasePlayerState* InOther);
	virtual void HandleReceiveInstantEvent(AMafiaBasePlayerState* InOther) { /**/ };
	virtual void HandleNotifyResultAbility(AMafiaBasePlayerState* InOther) { /**/ };
	virtual void HandleReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent);

	void HandleAbilityEvents();
	void HandleBroadCastEvents();

	virtual void HandleAbilityEvent(const FAffectedEvent& InEvent);
	virtual void HandleBroadCastEvent(const FBroadCastEvent& InEvent);
	

	virtual void HandleStartVoteEvent() { /**/ };
	virtual void HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag) { /**/ };
	virtual void HandleReceiveVoteResult(AMafiaBasePlayerState* InDeathRow, EMafiaVoteResultFlag InFlag) { /**/ };
	virtual void HandleFinishVoteEvent() { /**/ };

protected:
	class UMafiaChairmanManager* GetChairMan();

protected:
	UFUNCTION()
	virtual void OnChangedMafiaFlowState(const EMafiaFlowState& InMafiaFlowState);

	UFUNCTION()
	virtual void OnRep_Dead() PURE_VIRTUAL(UMafiaBaseGameInstance::OnRep_Dead, );



protected:
	UPROPERTY(Replicated)
	EMafiaTeam TeamType;

	UPROPERTY(Replicated)
	EMafiaRole RoleType;

	UPROPERTY()
	EMafiaAbilityEventType AbilityEventType;

	UPROPERTY(Replicated)
	FName RoleName;

protected:
	/** ktw : 내가 능력을 볼 수 있는 플레이어의 Unique ID. */
	UPROPERTY()
	TSet<uint32> VisiblePlayerSet;

	/** ktw : 이번 턴에 내가 처리할 능력 이벤트 목록. */
	UPROPERTY()
	TArray<FAffectedEvent> CachedAffectedEventsHeap;

	/** ktw : 이번 턴에 내가 처리할 전체 이벤트 목록. */
	UPROPERTY()
	TArray<FBroadCastEvent> CachedBroadCastEventsHeap;

	/** ktw : 처리된 능력 이벤트 목록 */
	UPROPERTY()
	TSet<EMafiaRole> CachedProcessedAbilityEventsSet;

	/** ktw : 처리된 전체 이벤트 목록 */
	UPROPERTY()
	TSet<EMafiaBroadCastEvent> CachedProcessedBroadCastEventsSet;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Dead)
	uint8 bDead : 1;

	UPROPERTY(Replicated)
	TWeakObjectPtr<class AMafiaBasePlayerState> OwningPlayerState;


private:
	float HandleAbilityEventTime;
	float HandleBroadCastEventTime;


private:
	FDelegateHandle OnChangedMafiaFlowStateHandle;
	FTimerHandle AffectedAbilityTimerHandle;
	FTimerHandle BroadCastEventTimerHandle;
};
