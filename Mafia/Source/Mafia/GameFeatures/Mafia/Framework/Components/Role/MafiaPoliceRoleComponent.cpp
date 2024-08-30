// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaPoliceRoleComponent.h"
#include "Mafia.h"

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

void UMafiaPoliceRoleComponent::ClientNotifyResultAbility_Implementation(UMafiaBaseRoleComponent* InOther)
{
	bool bAffectedByMadam = CachedAffectedEventsHeap.ContainsByPredicate([](const FAffectedEvent& Event) {
		return Event.Other->GetRoleType() == EMafiaRole::Madam;
	});

	if (bAffectedByMadam)
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("기생의 유혹에 당하여 능력 사용이 불가합니다."));
	}
	else
	{
		EMafiaTeam OtherTeam = InOther->GetTeamType();
		if (OtherTeam == EMafiaTeam::Mafia)
		{
			MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("%s 는 마피아 일원입니다. 경크를 외치세요!"), *InOther->GetRoleName().ToString());
		}
		else
		{
			MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("%s 는 시민입니다."), *InOther->GetRoleName().ToString());
		}
	}
}

void UMafiaPoliceRoleComponent::ClientAffectedEventsFlush_Implementation()
{
	
}

void UMafiaPoliceRoleComponent::ClientRecieveInstantEvent_Implementation(UMafiaBaseRoleComponent* InOther)
{
}

