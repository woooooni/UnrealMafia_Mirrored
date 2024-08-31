// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaBusDriverRoleComponent.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "Net/UnrealNetwork.h"

UMafiaBusDriverRoleComponent::UMafiaBusDriverRoleComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMafiaBusDriverRoleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMafiaBusDriverRoleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UMafiaBusDriverRoleComponent::UseAbility(AMafiaBasePlayerState* InOther)
{
	// Super 호출 금지.
	
}

void UMafiaBusDriverRoleComponent::HandleAffectedAbilities()
{
}

void UMafiaBusDriverRoleComponent::HandleNotifyResultAbility(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaBusDriverRoleComponent::HandleRecieveInstantEvent(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaBusDriverRoleComponent::HandleResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
}

void UMafiaBusDriverRoleComponent::HandleStartVoteEvent()
{
}

void UMafiaBusDriverRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
}

void UMafiaBusDriverRoleComponent::HandleReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
}

void UMafiaBusDriverRoleComponent::HandleFinishVoteEvent()
{
}


