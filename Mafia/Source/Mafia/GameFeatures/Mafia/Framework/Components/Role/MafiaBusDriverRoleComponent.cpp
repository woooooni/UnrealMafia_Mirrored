// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaBusDriverRoleComponent.h"
#include "Framework/Manager/MafiaChairManManager.h"
#include "MafiaCore/Framework/Player/MafiaBaseAbilityDwelling.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "Net/UnrealNetwork.h"

UMafiaBusDriverRoleComponent::UMafiaBusDriverRoleComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamType = EMafiaTeam::Citizen;
	RoleType = EMafiaRole::BusDriver;
	RoleName = TEXT("버스기사");
	AbilityEventType = EMafiaAbilityEventType::BusDriveEvent;

	BusData.BusDriver = this;
}

void UMafiaBusDriverRoleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMafiaBusDriverRoleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMafiaBusDriverRoleComponent, BusData);
	
}

void UMafiaBusDriverRoleComponent::BusDrive(UMafiaChairManManager* InContext)
{
	if (IsValid(InContext))
	{
		UMafiaBaseAbilityDwelling* FirstPassenger = BusData.FirstPassenger.Get();
		UMafiaBaseAbilityDwelling* SecondPassenger = BusData.SecondPassenger.Get();
		if (IsValid(FirstPassenger) && IsValid(SecondPassenger) && BusData.BusDriver.IsValid())
		{
			FirstPassenger->SetChangedPlayer(SecondPassenger->GetOriginPlayerState());
			SecondPassenger->SetChangedPlayer(FirstPassenger->GetOriginPlayerState());

			FirstPassenger->AddDeferredAbilityEvent(BusData.BusDriver.Get()->GetOwningPlayerState(), EMafiaAbilityEventType::DeferredEvent);
			SecondPassenger->AddDeferredAbilityEvent(BusData.BusDriver.Get()->GetOwningPlayerState(), EMafiaAbilityEventType::DeferredEvent);
		}
	}
}

EMafiaUseAbilityFlag UMafiaBusDriverRoleComponent::PickupPassenger(UMafiaBaseAbilityDwelling* InPassengerDwelling)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (BusData.FirstPassenger.IsValid() && BusData.SecondPassenger.IsValid())
		{
			/** ktw : 이미 탔던 승객. */
			if (BusData.FirstPassenger == InPassengerDwelling)
			{
				BusData.SecondPassenger = InPassengerDwelling;
				return EMafiaUseAbilityFlag::Succeed;
			}
			else
			{
				BusData.FirstPassenger = InPassengerDwelling;
				return EMafiaUseAbilityFlag::Succeed;
			}
		}
		else
		{
			/** ktw : 승객이 한 명만 있을 경우나, 아예 없을 경우. */
			if (BusData.FirstPassenger.IsValid())
			{
				BusData.SecondPassenger = InPassengerDwelling;
				return EMafiaUseAbilityFlag::Succeed;
			}
			else
			{
				BusData.FirstPassenger = InPassengerDwelling;
				return EMafiaUseAbilityFlag::Succeed;
			}
		}
	}
	

	return EMafiaUseAbilityFlag::Failed;
}


void UMafiaBusDriverRoleComponent::OnRep_Dead()
{
	
}

void UMafiaBusDriverRoleComponent::OnRep_BusData()
{

}


