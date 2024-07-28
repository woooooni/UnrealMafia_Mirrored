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
	RoleType = EMafiaRole::Mafia;
	RoleName = TEXT("마피아(대부)");
}

void UMafiaGodFatherRoleComponent::ClientAffectedEventsFlush_Implementation()
{
}

void UMafiaGodFatherRoleComponent::ClientResponsePostUseAbility_Implementation(UMafiaBaseRoleComponent* InOther)
{
}
