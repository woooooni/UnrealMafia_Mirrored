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
	void BusDrive(class UMafiaChairManManager* InContext);
	EMafiaUseAbilityFlag PickupPassenger(class UMafiaBaseAbilityDwelling* InPassengerDwelling);

protected:
	virtual void HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType) override;
	virtual	void HandleAffectedAbilities() override;
	virtual void HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther) override;
	virtual void HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther) override;
	
	virtual void HandleStartVoteEvent() override;
	virtual void HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag) override;
	virtual void HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag) override;
	virtual void HandleFinishVoteEvent() override;
	

protected:
	virtual void OnRep_Dead() override;

	UFUNCTION()
	void OnRep_BusData();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_BusData)
	FBusData BusData;

};
