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

void UMafiaCitizenRoleComponent::OnRep_Dead()
{
}


void UMafiaCitizenRoleComponent::ClientNotifyResultAbility_Implementation(UMafiaBaseRoleComponent* InOther)
{
}

void UMafiaCitizenRoleComponent::ClientAffectedEventsFlush_Implementation()
{
	/** ktw : 내가 능력을 볼 수 있는 플레이어의 Unique ID. */
	// VisiblePlayerSet;

	/** ktw : 이번 턴에 내가 처리할 이벤트 목록. */
	// CachedAffectedEventsHeap;

	while (!CachedAffectedEventsHeap.IsEmpty())
	{
		FAffectedEvent Event;
		CachedAffectedEventsHeap.HeapPop(Event);

		if (Event.Other.IsValid())
		{
			UMafiaBaseRoleComponent* Other = Event.Other.Get();
			EMafiaRole OtherRole = Other->GetRoleType();

			if (OtherRole == EMafiaRole::Madam)
			{
			}

			else if (OtherRole == EMafiaRole::Killer)
			{
			}

			else if (OtherRole == EMafiaRole::Mafia)
			{
			}

			else if (OtherRole == EMafiaRole::Vigilante)
			{
			}

			else if (OtherRole == EMafiaRole::BusDriver)
			{
			}

			else if (OtherRole == EMafiaRole::Detective)
			{
			}

			else if (OtherRole == EMafiaRole::Spirit)
			{
			}

			else if (OtherRole == EMafiaRole::Thief)
			{
			}

			else if (OtherRole == EMafiaRole::Soldier)
			{
			}

			else if (OtherRole == EMafiaRole::Doctor)
			{
			}
		}
	}

}
