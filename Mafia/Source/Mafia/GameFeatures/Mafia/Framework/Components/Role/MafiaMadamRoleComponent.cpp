// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaMadamRoleComponent.h"

UMafiaMadamRoleComponent::UMafiaMadamRoleComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamType = EMafiaTeam::Citizen;
	RoleType = EMafiaRole::Madam;
	RoleName = TEXT("마담");
}

void UMafiaMadamRoleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMafiaMadamRoleComponent::HandleAffectedAbilities()
{
}

void UMafiaMadamRoleComponent::HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaMadamRoleComponent::HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaMadamRoleComponent::HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
}

void UMafiaMadamRoleComponent::HandleStartVoteEvent()
{
}

void UMafiaMadamRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
}

void UMafiaMadamRoleComponent::HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
}

void UMafiaMadamRoleComponent::HandleFinishVoteEvent()
{
}

void UMafiaMadamRoleComponent::OnRep_Dead()
{
}
