// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaBusDriverRoleComponent.generated.h"

USTRUCT(BlueprintType)
struct FBusData
{
	GENERATED_BODY()

public:
	FORCEINLINE void Reset() {
		FirstPassenger = nullptr;
		SecondPassenger = nullptr;
	}

public:
	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseAbilityDwelling> FirstPassenger;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseAbilityDwelling> SecondPassenger;

	UPROPERTY()
	TWeakObjectPtr<UMafiaBusDriverRoleComponent> BusDriver;
};

/**
 * 
 */

UCLASS()
class MAFIA_API UMafiaBusDriverRoleComponent : public UMafiaBaseRoleComponent
{
	GENERATED_BODY()

public:
	UMafiaBusDriverRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//FORCEINLINE const FBusData& GetBusData() { return BusData; }

public:
	void BusDrive(class UMafiaChairmanManager* InContext);
	EMafiaUseAbilityFlag PickupPassenger(class UMafiaBaseAbilityDwelling* InPassengerDwelling);

protected:
	virtual void HandleResponseUseAbilityEvent(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType) override;

	virtual void HandleReceiveAffectedAbilityEvent(EMafiaRole InRole, AMafiaBasePlayerState* InOther) override;
	virtual void HandleReceiveInstantEvent(AMafiaBasePlayerState* InOther) override;
	virtual void HandleReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent) override;

	virtual void HandleNotifyResultAbilityEvent(AMafiaBasePlayerState* InOther) override;
	virtual void HandleAffectedAbilityEvent(const FAffectedEvent& InEvent) override;

	virtual void HandleBroadCastEvent(const FBroadCastEvent& InEvent) override;
	virtual void HandleStartVoteEvent() override;

	virtual void HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag) override;
	virtual void HandleReceiveVoteResultEvent(AMafiaBasePlayerState* InDeathRow, EMafiaVoteResultFlag InFlag) override;
	virtual void HandleFinishVoteEvent() override;
	

protected:
	virtual void OnRep_Dead() override;

	UFUNCTION()
	void OnRep_BusData();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_BusData)
	FBusData BusData;

};
