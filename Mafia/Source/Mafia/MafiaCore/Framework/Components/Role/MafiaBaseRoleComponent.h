// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Framework/Types/MafiaTypes.h"
#include "MafiaBaseRoleComponent.generated.h"

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

public:
	UFUNCTION()
	virtual void UseAbility(class AMafiaBasePlayerState* InOther);

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
	void RecieveInstantEvent(UMafiaBaseRoleComponent* InOther);


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

protected:
	UFUNCTION(Client, Reliable)
	void ClientAffectedAbilityByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther);

	UFUNCTION(Client, Reliable)
	virtual void ClientNotifyResultAbility(UMafiaBaseRoleComponent* InOther);

	UFUNCTION(Client, Reliable)
	virtual void ClientResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType);

	UFUNCTION(Client, Reliable)
	virtual void ClientAffectedEventsFlush();

	UFUNCTION(Client, Reliable)
	virtual void ClientRecieveInstantEvent(UMafiaBaseRoleComponent* InOther);
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
	virtual void OnRep_Dead() PURE_VIRTUAL(UMafiaBaseGameInstance::OnRep_Dead, );

	UFUNCTION()
	virtual void OnChangedMafiaFlowState(const EMafiaFlowState& InMafiaFlowState);

protected:
	UPROPERTY(Replicated)
	EMafiaTeam TeamType;

	UPROPERTY(Replicated)
	EMafiaRole RoleType;

	UPROPERTY(Replicated)
	FName RoleName;

protected:
	/** ktw : 내가 능력을 볼 수 있는 플레이어의 Unique ID. */
	UPROPERTY()
	TSet<uint32> VisiblePlayerSet;

	/** ktw : 이번 턴에 내가 처리할 이벤트 목록. */
	UPROPERTY(Replicated)
	TArray<FAffectedEvent> CachedAffectedEventsHeap;



private:
	UPROPERTY(ReplicatedUsing = OnRep_Dead)
	uint8 bDead : 1;

	UPROPERTY(Replicated)
	TWeakObjectPtr<class AMafiaBasePlayerState> OwningPlayerState;

private:
	FDelegateHandle OnChangedMafiaFlowStateHandle;
};
