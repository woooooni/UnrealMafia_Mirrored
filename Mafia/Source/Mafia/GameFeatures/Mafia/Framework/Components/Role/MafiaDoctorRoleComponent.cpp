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


void UMafiaDoctorRoleComponent::OnRep_Dead()
{
}
