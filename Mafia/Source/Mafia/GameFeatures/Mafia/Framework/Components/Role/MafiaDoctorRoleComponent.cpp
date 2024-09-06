// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaDoctorRoleComponent.h"

UMafiaDoctorRoleComponent::UMafiaDoctorRoleComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMafiaDoctorRoleComponent::BeginPlay()
{
	Super::BeginPlay();

	TeamType = EMafiaTeam::Citizen;
	RoleType = EMafiaRole::Doctor;
	AbilityEventType = EMafiaAbilityEventType::DeferredEvent;
	RoleName = TEXT("의사");
}

void UMafiaDoctorRoleComponent::HandleAffectedAbilities()
{
}

void UMafiaDoctorRoleComponent::HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	Super::HandleResponseUseAbility(InOther, InFlag, InEventType);
}

void UMafiaDoctorRoleComponent::HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaDoctorRoleComponent::HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther)
{
}


void UMafiaDoctorRoleComponent::HandleStartVoteEvent()
{
}

void UMafiaDoctorRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
}

void UMafiaDoctorRoleComponent::HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
}

void UMafiaDoctorRoleComponent::HandleFinishVoteEvent()
{
}

void UMafiaDoctorRoleComponent::OnRep_Dead()
{
}
