// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaGodFatherRoleComponent.h"

UMafiaGodFatherRoleComponent::UMafiaGodFatherRoleComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMafiaGodFatherRoleComponent::BeginPlay()
{
	Super::BeginPlay();

	TeamType = EMafiaTeam::Mafia;
	RoleType = EMafiaRole::GodFather;
	AbilityEventType = EMafiaAbilityEventType::DeferredEvent;
	RoleName = TEXT("마피아(대부)");
}

void UMafiaGodFatherRoleComponent::HandleAffectedAbilities()
{
}

void UMafiaGodFatherRoleComponent::HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	Super::HandleResponseUseAbility(InOther, InFlag, InEventType);
}

void UMafiaGodFatherRoleComponent::HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaGodFatherRoleComponent::HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaGodFatherRoleComponent::HandleStartVoteEvent()
{
}

void UMafiaGodFatherRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
}

void UMafiaGodFatherRoleComponent::HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
}

void UMafiaGodFatherRoleComponent::HandleFinishVoteEvent()
{
}

void UMafiaGodFatherRoleComponent::OnRep_Dead()
{
}
