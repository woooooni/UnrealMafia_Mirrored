// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaSoldierRoleComponent.h"

UMafiaSoldierRoleComponent::UMafiaSoldierRoleComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	TeamType = EMafiaTeam::Citizen;
	RoleType = EMafiaRole::Soldier;
	RoleName = TEXT("군인");
	BulletproofCount = 1;
}

void UMafiaSoldierRoleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMafiaSoldierRoleComponent::HandleAffectedAbilities()
{
}

void UMafiaSoldierRoleComponent::HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	Super::HandleResponseUseAbility(InOther, InFlag, InEventType);
}

void UMafiaSoldierRoleComponent::HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaSoldierRoleComponent::HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaSoldierRoleComponent::HandleStartVoteEvent()
{
}

void UMafiaSoldierRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
}

void UMafiaSoldierRoleComponent::HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
}

void UMafiaSoldierRoleComponent::HandleFinishVoteEvent()
{
}

void UMafiaSoldierRoleComponent::OnRep_Dead()
{
	Super::OnRep_Dead();
}
