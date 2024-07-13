// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Framework/Types/MafiaTypes.h"
#include "GameFeatures/Mafia/Gameflow/MafiaChairMan.h"
#include "MafiaBaseRoleComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MAFIA_API UMafiaBaseRoleComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UMafiaBaseRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	

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


	
public:
	UFUNCTION()
	void UseAbility(class AMafiaPlayerState* InOther);

	UFUNCTION()
	void AffectedByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther);
	

protected:
	UFUNCTION(Server, Reliable)
	void ServerReqSetTeam(EMafiaTeam InTeam);

	UFUNCTION(Server, Reliable)
	void ServerReqSetRole(EMafiaRole InRole);

	UFUNCTION(Server, Reliable)
	void ServerReqSetDead(bool InDead);

	/** ktw - 능력 사용 관련 함수들. */
	UFUNCTION(Server, Reliable)
	void ServerReqUseAbility(class AMafiaPlayerState* InOther);

protected:
	bool IsDedicatedServer();


protected:
	/** ktw - 내가 능력을 볼 수 있는 플레이어의 Unique ID. */
	TSet<uint32> VisiblePlayer;
	
	/** ktw - 이번 턴에 처리할 이벤트 목록. */
	TArray<FUseAbilityEvent> EffectedAbilityEventsHeap;


protected:
	UPROPERTY(Replicated)
	EMafiaTeam TeamType;

	UPROPERTY(Replicated)
	EMafiaRole RoleType;

	UPROPERTY(Replicated)
	uint8 bDead : 1;
	
	
	TWeakObjectPtr<AMafiaPlayerState> OwningPlayerState;
};
