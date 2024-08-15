// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaBusDriverRoleComponent.generated.h"

/**
 * 
 */

USTRUCT()
struct FBusPassengers
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<AMafiaBasePlayerState> First;

	UPROPERTY()
	TWeakObjectPtr<AMafiaBasePlayerState> Second;
};

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
	virtual void UseAbility(class AMafiaBasePlayerState* InOther) override;	
	virtual void BusDrive() override;

protected:
	virtual void OnRep_Dead() override;

protected:
	virtual void ClientNotifyResultAbility_Implementation(UMafiaBaseRoleComponent* InOther) override;
	virtual void ClientAffectedEventsFlush_Implementation() override;
	

private:
	UFUNCTION()
	void ChoicePassenger(class AMafiaBasePlayerState* InOther);

	UFUNCTION(Server, Reliable)
	void ServerReqChoicePassenger(class AMafiaBasePlayerState* InOther);




private:
	UPROPERTY(Replicated)
	FBusPassengers Passengers;

};
