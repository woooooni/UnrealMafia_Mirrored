// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaBusDriverRoleComponent.h"
#include "Framework/Manager/MafiaChairmanManager.h"
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


void UMafiaBusDriverRoleComponent::BusDrive(UMafiaChairmanManager* InContext)
{
	if (IsValid(InContext))
	{
		UMafiaBaseAbilityDwelling* FirstPassenger = BusData.FirstPassenger.Get();
		UMafiaBaseAbilityDwelling* SecondPassenger = BusData.SecondPassenger.Get();
		if (IsValid(FirstPassenger) && IsValid(SecondPassenger))
		{
			bool bSucceed = true;
			bSucceed &= FirstPassenger->SetChangedPlayer(SecondPassenger->GetOriginPlayerState());
			bSucceed &= SecondPassenger->SetChangedPlayer(FirstPassenger->GetOriginPlayerState());

			if (bSucceed == false)
			{
				FirstPassenger->ResetChangedPlayer();
				SecondPassenger->ResetChangedPlayer();
				return;
			}

			FirstPassenger->AddDeferredAbilityEvent(GetOwningPlayerState(), EMafiaAbilityEventType::DeferredEvent);
			SecondPassenger->AddDeferredAbilityEvent(GetOwningPlayerState(), EMafiaAbilityEventType::DeferredEvent);

			CheckTrafficAccident();
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

void UMafiaBusDriverRoleComponent::CheckTrafficAccident()
{
	UMafiaBaseAbilityDwelling* FirstPassenger = BusData.FirstPassenger.Get();
	UMafiaBaseAbilityDwelling* SecondPassenger = BusData.SecondPassenger.Get();

	if (IsValid(FirstPassenger) && IsValid(SecondPassenger))
	{
		UMafiaBaseRoleComponent* FirstRoleComponent = FirstPassenger->GetOriginPlayerRoleComponent();
		UMafiaBaseRoleComponent* SecondRoleComponent = SecondPassenger->GetOriginPlayerRoleComponent();

		if (IsValid(FirstRoleComponent) && IsValid(SecondRoleComponent))
		{
			EMafiaRole FirstRoleType = FirstRoleComponent->GetRoleType();
			EMafiaRole SecondRoleType = SecondRoleComponent->GetRoleType();

			bool bKillerRoleFlag = false;

			bKillerRoleFlag |= (FirstRoleType == EMafiaRole::GodFather);
			bKillerRoleFlag |= (FirstRoleType == EMafiaRole::Killer);
			bKillerRoleFlag |= (FirstRoleType == EMafiaRole::Vigilante);
			bKillerRoleFlag &= (FirstRoleType != EMafiaRole::GodFather);

			if (bKillerRoleFlag)
			{
				if (SecondPassenger->CheckAffectedOtherRole(FirstRoleType))
				{
					SendBroadCastEvent(EMafiaBroadCastEvent::TrafficAccident);
				}
			}

			bKillerRoleFlag = false;
			bKillerRoleFlag |= (SecondRoleType == EMafiaRole::GodFather);
			bKillerRoleFlag |= (SecondRoleType == EMafiaRole::Killer);
			bKillerRoleFlag |= (SecondRoleType == EMafiaRole::Vigilante);
			bKillerRoleFlag &= (SecondRoleType != EMafiaRole::GodFather);

			if (bKillerRoleFlag)
			{
				if (FirstPassenger->CheckAffectedOtherRole(SecondRoleType))
				{
					SendBroadCastEvent(EMafiaBroadCastEvent::TrafficAccident);
				}
			}
		}
	}

	return;
}

void UMafiaBusDriverRoleComponent::HandleResponseUseAbilityEvent(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	Super::HandleResponseUseAbilityEvent(InOther, InFlag, InEventType);
	if (InFlag == EMafiaUseAbilityFlag::Succeed)
	{
		EMafiaColor OtherColor = InOther->GetPlayerColor();
		MAFIA_ULOG(LogMafiaAbility, Log, TEXT("%s을 승객으로 태웠습니다."), *GPlayerColorKoreanNames[int32(OtherColor)].ToString());
	}
}

void UMafiaBusDriverRoleComponent::HandleReceiveAffectedAbilityEvent(EMafiaRole InRole, AMafiaBasePlayerState* InOther)
{
	Super::HandleReceiveAffectedAbilityEvent(InRole, InOther);
}

void UMafiaBusDriverRoleComponent::HandleReceiveInstantEvent(AMafiaBasePlayerState* InOther)
{
	Super::HandleReceiveInstantEvent(InOther);
}

void UMafiaBusDriverRoleComponent::HandleNotifyResultAbilityEvent(AMafiaBasePlayerState* InOther)
{
	Super::HandleNotifyResultAbilityEvent(InOther);
}

void UMafiaBusDriverRoleComponent::HandleReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent)
{
	Super::HandleReceiveBroadCastEvent(InSender, InEvent);
}

void UMafiaBusDriverRoleComponent::HandleAffectedAbilityEvent(const FAffectedEvent& InEvent)
{
	Super::HandleAffectedAbilityEvent(InEvent);
}

void UMafiaBusDriverRoleComponent::HandleBroadCastEvent(const FBroadCastEvent& InEvent)
{
	Super::HandleBroadCastEvent(InEvent);
}

void UMafiaBusDriverRoleComponent::HandleStartVoteEvent()
{
	Super::HandleStartVoteEvent();
}

void UMafiaBusDriverRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
	Super::HandleResponseVoteEvent(InCandidate, InFlag);
}

void UMafiaBusDriverRoleComponent::HandleReceiveVoteResultEvent(AMafiaBasePlayerState* InDeathRow, EMafiaVoteResultFlag InFlag)
{
	Super::HandleReceiveVoteResultEvent(InDeathRow, InFlag);
}

void UMafiaBusDriverRoleComponent::HandleFinishVoteEvent()
{
	Super::HandleFinishVoteEvent();
}


void UMafiaBusDriverRoleComponent::OnChangedMafiaFlowState(const EMafiaFlowState& InFlowState)
{
	Super::OnChangedMafiaFlowState(InFlowState);
	if (InFlowState == EMafiaFlowState::BeforeNight)
	{
		BusData.Reset();
	}
}

void UMafiaBusDriverRoleComponent::OnRep_Dead()
{
	Super::OnRep_Dead();
}

void UMafiaBusDriverRoleComponent::OnRep_BusData()
{
	
}


