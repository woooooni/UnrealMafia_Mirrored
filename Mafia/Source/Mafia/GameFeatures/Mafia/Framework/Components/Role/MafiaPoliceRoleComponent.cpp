// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaPoliceRoleComponent.h"
#include "Mafia.h"

UMafiaPoliceRoleComponent::UMafiaPoliceRoleComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMafiaPoliceRoleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TeamType = EMafiaTeam::Citizen;
	RoleType = EMafiaRole::Police;
	RoleName = TEXT("경찰");
}

void UMafiaPoliceRoleComponent::HandleAffectedAbilities()
{
}

void UMafiaPoliceRoleComponent::HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther)
{
	if (IsValid(InOther))
	{
		EMafiaRole OtherRole = InOther->GetRoleType();
	}
}

void UMafiaPoliceRoleComponent::HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaPoliceRoleComponent::HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
}

void UMafiaPoliceRoleComponent::HandleStartVoteEvent()
{
}

void UMafiaPoliceRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
}

void UMafiaPoliceRoleComponent::HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
}

void UMafiaPoliceRoleComponent::HandleFinishVoteEvent()
{
}

void UMafiaPoliceRoleComponent::OnRep_Dead()
{

}

