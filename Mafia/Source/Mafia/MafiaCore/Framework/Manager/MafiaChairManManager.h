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
	/** ktw : 능력을 사용한 플레이어의 역할 */
	UPROPERTY()
	EMafiaRole OriginRole;

	/** ktw : 능력을 사용한 플레이어 */
	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> OriginPlayer;

	/** ktw : 능력에 영향받는 플레이어 */
	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> DestPlayer;

public:
	/** ktw : Pred Definition */
	FORCEINLINE bool operator< (const FUseAbilityEventData& Other) const
	{
		return OriginRole > Other.OriginRole;
	}

	FORCEINLINE bool operator== (const FUseAbilityEventData& Other) const
	{
		return OriginRole == Other.OriginRole;
	}
};

USTRUCT()
struct FPlayerVoteData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Votor;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Candidate;

	UPROPERTY()
	uint8 VotedCount = 0;

public:
	/** ktw : Pred Definition */
	FORCEINLINE bool operator< (const FPlayerVoteData& Other) const
	{
		return VotedCount < Other.VotedCount;
	}
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
	bool AssigningAllPlayersAbility();

	/** 
		ktw : Origin -> Destination으로 향하는 능력 사용 이벤트를 Heap에 임시 저장합니다.  
		InOrigin : 능력 사용 Player, InDestination : 능력에 영향을 받은 Player.
	*/
	UFUNCTION()
	void AddAbilityEvent(class AMafiaBasePlayerState* InOrigin, class AMafiaBasePlayerState* InDestination);

	/** ktw : InVotor - 투표자, InCandidate - 피투표자 */
	UFUNCTION()
	void AddVoteEvent(class AMafiaBasePlayerState* InVotor, class AMafiaBasePlayerState* InCandidate);

	UFUNCTION()
	EMafiaGameResult CheckGameResult() const;

public:
	FORCEINLINE const TArray<FUseAbilityEventData>& GetPlayerAbilityDataHeap() { return CachedAbilityEventsHeap; }
	FORCEINLINE const TMap<FName, FPlayerVoteData>& GetPlayerVoteMap() { return CachedVoteEventsMap; }

#pragma region Cheat
public:
	UFUNCTION()
	void CheatChangeRole(AMafiaBasePlayerState* InPlayerState, UMafiaBaseRoleComponent* InNewRoleComponent);
#pragma endregion Cheat

public:
	UFUNCTION()
	void OnSetMafiaFlowState(EMafiaFlowState InFlowState);
	

private:
	/** ktw : Heap에 저장된 능력 이벤트들을 순회하면서 각 플레이어의 RoleComponent에 이벤트를 전송합니다. */
	UFUNCTION()
	void DispatchAbilityEvents();

	/** ktw : RoleComponent에 전송한 이벤트를 실행하는 신호를 보냅니다. */
	UFUNCTION()
	void FlushAbilityEvents() const;


private:
	UFUNCTION()
	void StartVote();
	
	UFUNCTION()
	class UMafiaBaseRoleComponent* FindDeadMan();

	UFUNCTION()
	EMafiaVoteResultFlag NotifyDeathRow();

	UFUNCTION()
	void EndVote();

private:
	bool MakeShuffledRoleArray(int32 InPlayerCount, OUT TArray<EMafiaRole>& OutSuffledArray);
	bool IsPossibleVote();

private:
	UFUNCTION()
	void NotifyGameResult(EMafiaGameResult InGameResult) const;


private:
	/** ktw : EMafiaRole에 선언된 Role의 값 순서로 Event를 정렬합니다. */
	UPROPERTY()
	TArray<FUseAbilityEventData> CachedAbilityEventsHeap;

	/** 
		ktw : 피 투표자를 저장합니다. 

		Key - 피 투표자 AccountId.
		Value - FPlayerVoteData.
	*/
	UPROPERTY()
	TMap<FName, FPlayerVoteData> CachedVoteEventsMap;

	/** ktw : 투표자를 저장합니다. */
	UPROPERTY()
	TSet<FName> CachedAlreadyVotersSet;

private:
	UPROPERTY()
	TMap<FName, TObjectPtr<UMafiaBaseRoleComponent>> JoinedPlayerRoleComponents;
	
	
};
