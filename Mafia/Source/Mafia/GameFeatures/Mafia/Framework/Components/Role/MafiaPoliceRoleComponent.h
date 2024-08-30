// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaPoliceRoleComponent.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaPoliceRoleComponent : public UMafiaBaseRoleComponent
{
	GENERATED_BODY()
	
public:
	UMafiaPoliceRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

protected:
	virtual void ClientNotifyResultAbility_Implementation(UMafiaBaseRoleComponent* InOther) override;
	virtual void ClientAffectedEventsFlush_Implementation() override;
	virtual void ClientRecieveInstantEvent_Implementation(UMafiaBaseRoleComponent* InOther) override;

protected:
	virtual void OnRep_Dead() override;
};
