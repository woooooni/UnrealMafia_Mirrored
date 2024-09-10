// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaDoctorRoleComponent.h"
#include "MafiaCore/Framework/Player/MafiaBaseAbilityDwelling.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"

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

void UMafiaDoctorRoleComponent::HandleResponseUseAbilityEvent(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	Super::HandleResponseUseAbilityEvent(InOther, InFlag, InEventType);
	if (InFlag == EMafiaUseAbilityFlag::Succeed)
	{
		EMafiaColor OtherColor = InOther->GetPlayerColor();
		MAFIA_ULOG(LogMafiaAbility, Log, TEXT("%s을(를) 살리기로 결정했습니다."), *GPlayerColorKoreanNames[int32(OtherColor)].ToString());
	}
}

void UMafiaDoctorRoleComponent::HandleReceiveAffectedAbilityEvent(EMafiaRole InRole, AMafiaBasePlayerState* InOther)
{
	Super::HandleReceiveAffectedAbilityEvent(InRole, InOther);
}

void UMafiaDoctorRoleComponent::HandleReceiveInstantEvent(AMafiaBasePlayerState* InOther)
{
	Super::HandleReceiveInstantEvent(InOther);
}

void UMafiaDoctorRoleComponent::HandleNotifyResultAbilityEvent(AMafiaBasePlayerState* InOther)
{
	Super::HandleNotifyResultAbilityEvent(InOther);
}

void UMafiaDoctorRoleComponent::HandleReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent)
{
	Super::HandleReceiveBroadCastEvent(InSender, InEvent);
}

void UMafiaDoctorRoleComponent::HandleAffectedAbilityEvent(const FAffectedEvent& InEvent)
{
	Super::HandleAffectedAbilityEvent(InEvent);
}

void UMafiaDoctorRoleComponent::HandleBroadCastEvent(const FBroadCastEvent& InEvent)
{
	Super::HandleBroadCastEvent(InEvent);
}

void UMafiaDoctorRoleComponent::HandleStartVoteEvent()
{
	Super::HandleStartVoteEvent();
}

void UMafiaDoctorRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
	Super::HandleResponseVoteEvent(InCandidate, InFlag);
}

void UMafiaDoctorRoleComponent::HandleReceiveVoteResultEvent(AMafiaBasePlayerState* InDeathRow, EMafiaVoteResultFlag InFlag)
{
	Super::HandleReceiveVoteResultEvent(InDeathRow, InFlag);
}

void UMafiaDoctorRoleComponent::HandleFinishVoteEvent()
{
	Super::HandleFinishVoteEvent();
}

void UMafiaDoctorRoleComponent::OnRep_Dead()
{
}
