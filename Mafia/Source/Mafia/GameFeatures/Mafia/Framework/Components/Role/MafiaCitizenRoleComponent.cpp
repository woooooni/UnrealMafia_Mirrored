// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaCitizenRoleComponent.h"

UMafiaCitizenRoleComponent::UMafiaCitizenRoleComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UMafiaCitizenRoleComponent::BeginPlay()
{
	Super::BeginPlay();

	TeamType = EMafiaTeam::Citizen;
	RoleType = EMafiaRole::Citizen;
	AbilityEventType = EMafiaAbilityEventType::DeferredEvent;
	RoleName = TEXT("시민");
}

void UMafiaCitizenRoleComponent::HandleAffectedAbilities()
{
	Super::HandleAffectedAbilities();
}

void UMafiaCitizenRoleComponent::HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	Super::HandleResponseUseAbility(InOther, InFlag, InEventType);
}

void UMafiaCitizenRoleComponent::HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaCitizenRoleComponent::HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther)
{
}



void UMafiaCitizenRoleComponent::HandleStartVoteEvent()
{
}

void UMafiaCitizenRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
}

void UMafiaCitizenRoleComponent::HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
}

void UMafiaCitizenRoleComponent::HandleFinishVoteEvent()
{
}

void UMafiaCitizenRoleComponent::OnRep_Dead()
{
}

