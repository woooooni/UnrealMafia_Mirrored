// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaBusDriverRoleComponent.generated.h"

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
	virtual void UseAbility(class AMafiaBasePlayerState* InOther) override;	

protected:
	virtual void ClientNotifyResultAbility_Implementation(UMafiaBaseRoleComponent* InOther) override;
	virtual void ClientAffectedEventsFlush_Implementation() override;

protected:
	virtual void OnRep_Dead() override;

};
