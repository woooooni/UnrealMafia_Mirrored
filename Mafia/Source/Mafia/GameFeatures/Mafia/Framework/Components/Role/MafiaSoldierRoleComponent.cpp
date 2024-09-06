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


void UMafiaSoldierRoleComponent::OnRep_Dead()
{
	Super::OnRep_Dead();
}
