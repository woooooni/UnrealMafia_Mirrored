// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaPoliceRoleComponent.h"

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

void UMafiaPoliceRoleComponent::OnRep_Dead()
{

}

void UMafiaPoliceRoleComponent::ClientAffectedEventsFlush_Implementation()
{
	
}

