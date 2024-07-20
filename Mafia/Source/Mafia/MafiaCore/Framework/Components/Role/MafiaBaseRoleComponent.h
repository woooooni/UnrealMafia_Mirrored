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
	friend bool operator< (const FAffectedEvent& A, const FAffectedEvent& B);
	
};

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MAFIA_API UMafiaBaseRoleComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UMafiaBaseRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

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
	void UseAbility(class AMafiaPlayerState* InOther);
	
public:
	/** ktw - 아래 함수들은 모두 서버에서 호출해야 합니다. */
	UFUNCTION()
	void AffectedByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther);
	
	/** ktw - 아래 함수들은 모두 서버에서 호출해야 합니다. */
	UFUNCTION()
	void FlushEvents();



protected:
	class UMafiaBaseGameInstance* GetServerInstance();

private:
	UFUNCTION(Server, Reliable)
	void ServerReqSetTeam(EMafiaTeam InTeam);

	UFUNCTION(Server, Reliable)
	void ServerReqSetRole(EMafiaRole InRole);

	UFUNCTION(Server, Reliable)
	void ServerReqSetDead(bool InDead);

	UFUNCTION(Server, Reliable)
	void ServerReqSetRoleName(FName InRoleName);

	/** ktw - 능력 사용 관련 함수들. */
	UFUNCTION(Server, Reliable)
	void ServerReqUseAbility(class AMafiaPlayerState* InOther);

	UFUNCTION(Client, Reliable)
	void ClientAffectedByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther);

	UFUNCTION(Client, Reliable)
	void ClientFlush();

private:
	UFUNCTION()
	virtual void OnRepChangeRoleType();


protected:
	/** ktw - 내가 능력을 볼 수 있는 플레이어의 Unique ID. */
	TSet<uint32> VisiblePlayer;
	
	/** ktw - 이번 턴에 내가 처리할 이벤트 목록. */
	TArray<FAffectedEvent> CachedAffectedEventsHeap;


protected:
	UPROPERTY(Replicated)
	EMafiaTeam TeamType;

	UPROPERTY(Replicated, ReplicatedUsing = OnRepChangeRoleType)
	EMafiaRole RoleType;

	UPROPERTY(Replicated)
	FName RoleName;

private:
	UPROPERTY(Replicated)
	uint8 bDead : 1;

	
	TWeakObjectPtr<class AMafiaBasePlayerState> OwningPlayerState;
};
