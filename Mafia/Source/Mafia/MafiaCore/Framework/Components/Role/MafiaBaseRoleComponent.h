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

public:
	UFUNCTION()
	void UseAbility(class AMafiaBasePlayerState* InOther);

	UFUNCTION()
	void Vote(class AMafiaBasePlayerState* InOther);
	

	UFUNCTION()
	FORCEINLINE class AMafiaBasePlayerState* GetOwningPlayerState() const { return OwningPlayerState.Get(); }

#pragma region Role Ability(역할 능력 관련)
public:
	/** 
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void AffectedAbilityByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther);

	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void ResponsePostUseAbility(UMafiaBaseRoleComponent* InOther);

	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void FlushAbilityEvents();
#pragma endregion Role Ability(역할 능력 관련)
	
#pragma region Vote(투표)
public:
	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void PreVoteEvent();

	/** 
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void ResponseVoteEvent(UMafiaBaseRoleComponent* InCandidate, EMafiaVoteFlag InFlag);

	/**
		ktw : 서버에서 호출해야합니다.
	*/
	UFUNCTION()
	void PostVoteEvent();
#pragma endregion Vote





#pragma region Role Ability RPC
private:
	UFUNCTION(Server, Reliable)
	void ServerReqUseAbility(AMafiaBasePlayerState* InOther);

protected:
	UFUNCTION(Client, Reliable)
	void ClientAffectedAbilityByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther);

	UFUNCTION(Client, Reliable)
	virtual void ClientResponsePostUseAbility(UMafiaBaseRoleComponent* InOther);

	UFUNCTION(Client, Reliable)
	virtual void ClientAffectedEventsFlush();
#pragma endregion Role Ability RPC

#pragma region Vote(투표) RPC
private:
	UFUNCTION(Server, Reliable)
	void ServerReqVote(AMafiaBasePlayerState* InOther);

	UFUNCTION(Client, Reliable)
	void ClientPreVoteEvent();

	UFUNCTION(Client, Reliable)
	void ClientResponseVoteEvent(UMafiaBaseRoleComponent* InCandidate, EMafiaVoteFlag InFlag);

	UFUNCTION(Client, Reliable)
	void ClientPostVoteEvent();
#pragma endregion Vote(투표)


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
	class UMafiaBaseGameInstance* GetServerInstance();

protected:
	UPROPERTY(Replicated)
	EMafiaTeam TeamType;

	UPROPERTY(Replicated)
	EMafiaRole RoleType;

	UPROPERTY(Replicated)
	FName RoleName;

protected:
	/** ktw : 내가 능력을 볼 수 있는 플레이어의 Unique ID. */
	TSet<uint32> VisiblePlayerSet;

	/** ktw : 이번 턴에 내가 처리할 이벤트 목록. */
	TArray<FAffectedEvent> CachedAffectedEventsHeap;

private:
	UPROPERTY(Replicated)
	uint8 bDead : 1;

	UPROPERTY(Replicated)
	TWeakObjectPtr<class AMafiaBasePlayerState> OwningPlayerState;
};
