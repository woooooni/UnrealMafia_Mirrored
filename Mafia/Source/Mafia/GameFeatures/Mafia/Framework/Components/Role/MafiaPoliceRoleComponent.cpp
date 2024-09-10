// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaPoliceRoleComponent.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
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
	AbilityEventType = EMafiaAbilityEventType::DeferredEvent;
	RoleName = TEXT("경찰");
}


void UMafiaPoliceRoleComponent::HandleResponseUseAbilityEvent(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	Super::HandleResponseUseAbilityEvent(InOther, InFlag, InEventType);
	if (InFlag == EMafiaUseAbilityFlag::Succeed)
	{
		EMafiaColor OtherColor = InOther->GetPlayerColor();
		MAFIA_ULOG(LogMafiaAbility, Log, TEXT("%s 을(를) 조사하기로 결정했습니다."), *GPlayerColorKoreanNames[int32(OtherColor)].ToString());
	}
}

void UMafiaPoliceRoleComponent::HandleReceiveAffectedAbilityEvent(EMafiaRole InRole, AMafiaBasePlayerState* InOther)
{
	Super::HandleReceiveAffectedAbilityEvent(InRole, InOther);
}

void UMafiaPoliceRoleComponent::HandleReceiveInstantEvent(AMafiaBasePlayerState* InOther)
{
	Super::HandleReceiveInstantEvent(InOther);
}

void UMafiaPoliceRoleComponent::HandleNotifyResultAbilityEvent(AMafiaBasePlayerState* InOther)
{
	Super::HandleNotifyResultAbilityEvent(InOther);
	if (IsValid(InOther))
	{
		FString ResultStr = TEXT("");

		if (UMafiaBaseRoleComponent* OtherComponent = InOther->GetRoleComponent())
		{
			EMafiaRole OtherRole = OtherComponent->GetRoleType();
			EMafiaTeam OtherTeam = OtherComponent->GetTeamType();
			EMafiaColor OtherColor = InOther->GetPlayerColor();

			if (OtherTeam == EMafiaTeam::Mafia)
			{
				if (OtherRole == EMafiaRole::GodFather)
				{
					MAFIA_ULOG(LogMafiaAbility, Log, TEXT("%s의 정체는 시민입니다."), *GPlayerColorKoreanNames[int32(OtherColor)].ToString());
				}
				else
				{
					MAFIA_ULOG(LogMafiaAbility, Log, TEXT("%s은(는) 정체가 의심스럽습니다."), *GPlayerColorKoreanNames[int32(OtherColor)].ToString());
				}
			}
			else if(OtherTeam == EMafiaTeam::Neutral)
			{
				MAFIA_ULOG(LogMafiaAbility, Log, TEXT("%s은(는) 정체가 의심스럽습니다."), *GPlayerColorKoreanNames[int32(OtherColor)].ToString());
			}
			else
			{
				MAFIA_ULOG(LogMafiaAbility, Log, TEXT("%s의 정체는 시민입니다."), *GPlayerColorKoreanNames[int32(OtherColor)].ToString());
			}			
		}
	}
	
}

void UMafiaPoliceRoleComponent::HandleReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent)
{
	Super::HandleReceiveBroadCastEvent(InSender, InEvent);
}

void UMafiaPoliceRoleComponent::HandleAffectedAbilityEvent(const FAffectedEvent& InEvent)
{
	Super::HandleAffectedAbilityEvent(InEvent);
}

void UMafiaPoliceRoleComponent::HandleBroadCastEvent(const FBroadCastEvent& InEvent)
{
	Super::HandleBroadCastEvent(InEvent);
}

void UMafiaPoliceRoleComponent::HandleStartVoteEvent()
{
	Super::HandleStartVoteEvent();
}

void UMafiaPoliceRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
	Super::HandleResponseVoteEvent(InCandidate, InFlag);
}

void UMafiaPoliceRoleComponent::HandleReceiveVoteResultEvent(AMafiaBasePlayerState* InDeathRow, EMafiaVoteResultFlag InFlag)
{
	Super::HandleReceiveVoteResultEvent(InDeathRow, InFlag);
}

void UMafiaPoliceRoleComponent::HandleFinishVoteEvent()
{
	Super::HandleFinishVoteEvent();
}



void UMafiaPoliceRoleComponent::OnRep_Dead()
{

}

