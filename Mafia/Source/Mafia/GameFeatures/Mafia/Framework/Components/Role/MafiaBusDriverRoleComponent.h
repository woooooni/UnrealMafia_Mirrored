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
	UMafiaBaseRoleComponent* FirstPassenger;

	UPROPERTY()
	UMafiaBaseRoleComponent* SecondPassenger;
};

UCLASS()
class MAFIA_API UMafiaBusDriverRoleComponent : public UMafiaBaseRoleComponent
{
	GENERATED_BODY()

public:
	UMafiaBusDriverRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:
	virtual void BusDrive() override;

protected:
	virtual void OnRep_Dead() override;

protected:
	virtual void ClientAffectedEventsFlush_Implementation() override;

private:
	UPROPERTY(Replicated)
	FBusPassengers Passengers;

};
