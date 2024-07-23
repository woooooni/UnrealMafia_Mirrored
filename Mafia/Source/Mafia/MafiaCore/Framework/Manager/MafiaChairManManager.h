// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaChairManManager.generated.h"


USTRUCT()
struct FUseAbilityEventData
{
	GENERATED_BODY()

public:
	EMafiaRole Role;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Origin;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Destination;

public:
	/** ktw : Pred Definition */
	FORCEINLINE bool operator< (const FUseAbilityEventData& Other) const
	{
		return Role > Other.Role;
	}
};

USTRUCT()
struct FPlayerVoteData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Origin;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Destination;

	uint8 VotedCount = 0;

};



/**
 *	ktw : Server에 존재하는 GameInstance만 이 객체를 가지고 있습니다. 따라서, 멤버 함수는 서버에서만 호출됩니다.
 */
UCLASS()
class MAFIA_API UMafiaChairManManager final : public UObject
{
	GENERATED_BODY()

public:
	UMafiaChairManManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION()
	void AssigningAllPlayersAbility();

public:
	/** ktw : Origin -> Destination으로 향하는 능력 사용 이벤트를 Heap에 임시 저장합니다.  */
	// InOrigin : 능력 사용 Player, InDestination : 능력에 영향을 받은 Player.
	UFUNCTION()
	void AddAbilityEvent(class AMafiaBasePlayerState* InOrigin, class AMafiaBasePlayerState* InDestination);

	/** ktw : Heap에 저장된 능력 이벤트들을 순회하면서 각 RoleComponent에 이벤트를 전송합니다. */
	UFUNCTION()
	void DispatchAbilityEvents();

	/** ktw : RoleComponent에 전송한 이벤트를 실행하는 신호를 보냅니다. */
	UFUNCTION()
	void RequestFlushAbilityEvents() const;


public:
	UFUNCTION()
	void StartVote();

	UFUNCTION()
	void AddVoteEvent(class UMafiaBaseRoleComponent* InOrigin, class UMafiaBaseRoleComponent* InDestination);

	UFUNCTION()
	void EndVote();

public:
	FORCEINLINE const TMap<FName, FPlayerVoteData>& GetPlayerVoteMap() { return CachedVoteMap; }


protected:
	bool MakeShuffledRoleArray(int32 InPlayerCount, OUT TArray<EMafiaRole>& OutSuffledArray);
	bool IsPossibleVote();

private:
	/** ktw : EMafiaRole에 선언된 Role의 값을 순서로 Event를 정렬합니다. */
	TArray<FUseAbilityEventData> CachedAbilityEventsHeap;
	TMap<FName, FPlayerVoteData> CachedVoteMap;
};
