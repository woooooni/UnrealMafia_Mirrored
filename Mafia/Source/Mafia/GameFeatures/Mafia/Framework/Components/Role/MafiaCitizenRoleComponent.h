// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaCitizenRoleComponent.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaCitizenRoleComponent : public UMafiaBaseRoleComponent
{
	GENERATED_BODY()

public:
	UMafiaCitizenRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnRepDead() override;

protected:
	virtual void ClientAffectedEventsFlush_Implementation() override;
	virtual void ClientResponsePostUseAbility_Implementation(UMafiaBaseRoleComponent* InOther) override;
	
};