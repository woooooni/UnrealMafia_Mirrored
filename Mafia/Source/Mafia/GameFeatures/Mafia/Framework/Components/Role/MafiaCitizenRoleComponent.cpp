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
	RoleName = TEXT("시민");
}

void UMafiaCitizenRoleComponent::ClientAffectedEventsFlush_Implementation()
{


}

void UMafiaCitizenRoleComponent::ClientResponsePostUseAbility_Implementation(UMafiaBaseRoleComponent* InOther)
{

}
