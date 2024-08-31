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
	virtual	void HandleAffectedAbilities() override;
	virtual void HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther);
	virtual void HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther);
	virtual void HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType);

	virtual void HandleStartVoteEvent() override;
	virtual void HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag) override;
	virtual void HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag) override;
	virtual void HandleFinishVoteEvent() override;

protected:
	virtual void OnRep_Dead() override;
	
};