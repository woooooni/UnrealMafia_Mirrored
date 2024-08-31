// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaDoctorRoleComponent.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaDoctorRoleComponent : public UMafiaBaseRoleComponent
{
	GENERATED_BODY()
	
public:
	UMafiaDoctorRoleComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

protected:
	virtual	void HandleAffectedAbilities() override;
	virtual void HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther) override;
	virtual void HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther) override;
	virtual void HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType) override;

	virtual void HandleStartVoteEvent() override;
	virtual void HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag) override;
	virtual void HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag) override;
	virtual void HandleFinishVoteEvent() override;

protected:
	virtual void OnRep_Dead() override;
};
