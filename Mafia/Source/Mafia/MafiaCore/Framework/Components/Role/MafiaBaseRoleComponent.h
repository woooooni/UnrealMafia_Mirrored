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
	TWeakObjectPtr<class UMafiaBaseRoleComponent> EventSender;

	UPROPERTY()
	EMafiaBroadCastEvent EventType;
};


USTRUCT()
struct FAffectedEvent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Other;

public:
	/** ktw - Pred Definition */
	friend bool operator < (const FAffectedEvent& A, const FAffectedEvent& B);
	
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
	void SetAffectedEvents(const TArray<FAffectedEvent>& InEvents);
	FORCEINLINE const TArray<FAffectedEvent>& GetAffectedEvents() { return CachedAffectedEventsHeap; }

	UFUNCTION()
	FORCEINLINE class AMafiaBasePlayerState* GetOwningPlayerState() const { return OwningPlayerState.Get(); }
#pragma endregion Getter & Setter


protected:
	UFUNCTION()
	virtual void BindDelegates();

	UFUNCTION()
	virtual void UnBindDelegates();

	UFUNCTION()
	virtual void ResetForNextRound();

public:
	UFUNCTION()
	virtual void UseAbility(class AMafiaBasePlayerState* InOther);

	UFUNCTION()
	void SendBroadCastEvent(const EMafiaBroadCastEvent& InEvent);

	UFUNCTION()
	void ReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent);

	UFUNCTION()
	virtual void SendOfferMafiaTeam(class AMafiaBasePlayerState* InOther);

	UFUNCTION()
	void Vote(class AMafiaBasePlayerState* InOther);
	

	

#pragma region Role Ability(역할 능력 관련)
public:
	/** 
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void AffectedAbilityByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther);

	/**
		ktw : 서버에서 호출해야 합니다.
	*/
	UFUNCTION()
	void ReceiveInstantEvent(UMafiaBaseRoleComponent* InOther);


	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void NotifyResultAbility(UMafiaBaseRoleComponent* InOther);
#pragma endregion Role Ability(역할 능력 관련)
	
#pragma region Vote(투표)
public:
	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void StartVoteEvent();

	/** 
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void ResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag);

	UFUNCTION()
	void ReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag);

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
	void ServerReqSendOfferMafiaTeam(AMafiaBasePlayerState* InOther);

	UFUNCTION(Server, Reliable)
	void ServerReqSetAffectedEvents(const TArray<FAffectedEvent>& InEvents);

	UFUNCTION(Server, Reliable)
	void ServerReqSendBroadCastEvent(const EMafiaBroadCastEvent InEvent);

protected:
	UFUNCTION(Client, Reliable)
	void ClientAffectedAbilityByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther);

	UFUNCTION(Client, Reliable)
	void ClientNotifyResultAbility(UMafiaBaseRoleComponent* InOther);

	UFUNCTION(Client, Reliable)
	void ClientResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType);

	UFUNCTION(Client, Reliable)
	void ClientAbilityEventsFlush();

	UFUNCTION(Client, Reliable)
	void ClientReceiveInstantEvent(UMafiaBaseRoleComponent* InOther);

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
	void ClientReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag);

	UFUNCTION(Client, Reliable)
	void ClientFinishVoteEvent();
#pragma endregion Vote(투표)


protected:
	class UMafiaChairManManager* GetChairMan();

private:
	UFUNCTION(Server, Reliable)
	void ServerReqSetTeam(EMafiaTeam InTeam);

	UFUNCTION(Server, Reliable)
	void ServerReqSetRole(EMafiaRole InRole);

	UFUNCTION(Server, Reliable)
	void ServerReqSetDead(bool InDead);

	UFUNCTION(Server, Reliable)
	void ServerReqSetRoleName(FName InRoleName);

protected:
	UFUNCTION()
	virtual void OnChangedMafiaFlowState(const EMafiaFlowState& InMafiaFlowState);

	UFUNCTION()
	virtual void OnRep_Dead() PURE_VIRTUAL(UMafiaBaseGameInstance::OnRep_Dead, );

protected:
	virtual void HandleReciveReciveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent);
	virtual	void HandleAffectedAbilities();
	virtual void HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther) { /**/ };
	virtual void HandleReceiveInstantEvent(UMafiaBaseRoleComponent* InOther) { /**/ };
	virtual void HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType);
	
	virtual void HandleStartVoteEvent() { /**/ };
	virtual void HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag) { /**/ };
	virtual void HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag) { /**/ };
	virtual void HandleFinishVoteEvent() { /**/ };



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
	TArray<FBroadCastEvent> CachedBroadCastEventArray;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Dead)
	uint8 bDead : 1;

	UPROPERTY(Replicated)
	TWeakObjectPtr<class AMafiaBasePlayerState> OwningPlayerState;


private:
	FDelegateHandle OnChangedMafiaFlowStateHandle;
	FTimerHandle AffectedAbilityTimerHandle;
};
