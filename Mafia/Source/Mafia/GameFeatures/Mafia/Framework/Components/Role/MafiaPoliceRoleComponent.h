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
};
