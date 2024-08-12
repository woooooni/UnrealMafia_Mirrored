// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaGodFatherRoleComponent.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaGodFatherRoleComponent final : public UMafiaBaseRoleComponent
{
	GENERATED_BODY()
	
public:
	UMafiaGodFatherRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnRep_Dead() override;

protected:
	virtual void ClientAffectedEventsFlush_Implementation() override;
};
