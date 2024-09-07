// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaChairmanManager.generated.h"




USTRUCT()
struct FPlayerVoteData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> Votor;

	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> Candidate;

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
class MAFIA_API UMafiaChairmanManager final : public UObject
{
	GENERATED_BODY()

public:
	UMafiaChairmanManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	bool StartGame();
	void EndGame();

public:
	/** 
		ktw : Origin -> Destination으로 향하는 능력 사용 이벤트를 Heap에 임시 저장합니다.  
		InOrigin : 능력 사용 Player, InDestination : 능력에 영향을 받은 Player.
	*/
	EMafiaUseAbilityFlag AddAbilityEvent(class AMafiaBasePlayerState* InOrigin, class AMafiaBasePlayerState* InDestination, EMafiaAbilityEventType InEventType);
	EMafiaBroadCastEventFlag BroadCastEvent(class AMafiaBasePlayerState* InSender, EMafiaBroadCastEvent InEvent);
	EMafiaUseAbilityFlag PickupPassenger(class UMafiaBaseAbilityDwelling* InPassengerDwelling);


	/** ktw : InVotor - 투표자, InCandidate - 피투표자 */
	EMafiaVoteFlag AddVoteEvent(class AMafiaBasePlayerState* InVotor, class AMafiaBasePlayerState* InCandidate);

	class AMafiaBasePlayerState* FindDeadMan();
	void NotifyDeadMan(class AMafiaBasePlayerState* InDeadMan);

	EMafiaGameResult CheckGameResult() const;

public:
	FORCEINLINE const TMap<FName, FPlayerVoteData>& GetPlayerVoteMap() { return CachedVoteEventsMap; }

#pragma region Cheat
public:
	UFUNCTION()
	void CheatChangeRole(AMafiaBasePlayerState* InPlayerState, UMafiaBaseRoleComponent* InNewRoleComponent);
#pragma endregion Cheat

public:
	void OnSetMafiaFlowState(EMafiaFlowState InFlowState);

private:
	bool AssignAllPlayersAbility();
	bool InitializePlayersAbilityDwelling();

private:
	void StartAbilityEvent();
	void PreBroadcastAbilityEvents();
	void BusDrive();
	void BroadcastAbilityEvents();
	void PostBroadcastAbilityEvents();
	void EndAbilityEvents();


private:
	void StartVote();
	void EndVote();

private:
	bool MakeShuffledRoleArray(int32 InPlayerCount, OUT TArray<EMafiaRole>& OutSuffledArray);
	bool IsPossibleVote();

private:
	void NotifyGameResult(EMafiaGameResult InGameResult) const;

private:
	void ResetForNextRound();

private:
	/** ktw : 능력사용 플레이어를 저장합니다. */
	UPROPERTY()
	TSet<FName> CachedAlreadyDeferredAbillityPlayerSet;

	/** ktw : 인스턴스 능력사용 플레이어를 저장합니다.*/
	UPROPERTY()
	TSet<FName> CachedAlreadyInstantAbillityPlayerSet;

	/** ktw : 투표자를 저장합니다. */
	UPROPERTY()
	TSet<FName> CachedAlreadyVoterSet;

	UPROPERTY()
	TMap<FName, FPlayerVoteData> CachedVoteEventsMap;



private:
	UPROPERTY()
	TMap<FName, TObjectPtr<class UMafiaBaseAbilityDwelling>> JoinedPlayerAbilityDwellings;

	UPROPERTY()
	TMap<FName, TObjectPtr<class UMafiaBaseRoleComponent>> JoinedPlayerRoleComponents;

	
private:
	TWeakObjectPtr<class UMafiaBusDriverRoleComponent> BusDriver;
};
