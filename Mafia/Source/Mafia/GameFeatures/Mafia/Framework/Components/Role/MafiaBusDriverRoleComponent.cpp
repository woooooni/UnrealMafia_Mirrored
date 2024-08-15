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
	DOREPLIFETIME(UMafiaBusDriverRoleComponent, Passengers);
}

void UMafiaBusDriverRoleComponent::UseAbility(AMafiaBasePlayerState* InOther)
{
	// Super 호출 금지.
	ChoicePassenger(InOther);
}

void UMafiaBusDriverRoleComponent::BusDrive()
{
	/** ktw : 버스 운행 시작 */
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Super::BusDrive();
		if (Passengers.First.IsValid() && Passengers.Second.IsValid())
		{
			UMafiaBaseRoleComponent* FirstPassenger = Passengers.First.Get()->GetRoleComponent();
			UMafiaBaseRoleComponent* SecondPassenger = Passengers.Second.Get()->GetRoleComponent();

			if (IsValid(FirstPassenger) && IsValid(SecondPassenger))
			{
				// FirstPassenger->BusRide();
				// SecondPassenger->BusRide();
			}
		}

		Passengers.First.Reset();
		Passengers.Second.Reset();
	}
}

void UMafiaBusDriverRoleComponent::OnRep_Dead()
{

}

void UMafiaBusDriverRoleComponent::ClientNotifyResultAbility_Implementation(UMafiaBaseRoleComponent* InOther)
{

}

void UMafiaBusDriverRoleComponent::ClientAffectedEventsFlush_Implementation()
{

}

void UMafiaBusDriverRoleComponent::ChoicePassenger(AMafiaBasePlayerState* InOther)
{
	ServerReqChoicePassenger(InOther);
}

void UMafiaBusDriverRoleComponent::ServerReqChoicePassenger_Implementation(AMafiaBasePlayerState* InOther)
{
	if (IsValid(InOther))
	{
		if (Passengers.First.IsValid())
		{
			/** 승객이 다 찼다! */
			Passengers.Second = InOther;
			Super::UseAbility(InOther);
		}
		else
		{
			/** 첫 번째 승객 등록 */
			Passengers.First = InOther;
		}
	}
}
