// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaMadamRoleComponent.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"

UMafiaMadamRoleComponent::UMafiaMadamRoleComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamType = EMafiaTeam::Citizen;
	RoleType = EMafiaRole::Madam;
	AbilityEventType = EMafiaAbilityEventType::DeferredEvent;
	RoleName = TEXT("마담");
}

void UMafiaMadamRoleComponent::BeginPlay()
{
	Super::BeginPlay();
}



void UMafiaMadamRoleComponent::OnRep_Dead()
{
}
