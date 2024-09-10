// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaGodFatherRoleComponent.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"

UMafiaGodFatherRoleComponent::UMafiaGodFatherRoleComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMafiaGodFatherRoleComponent::BeginPlay()
{
	Super::BeginPlay();

	TeamType = EMafiaTeam::Mafia;
	RoleType = EMafiaRole::GodFather;
	AbilityEventType = EMafiaAbilityEventType::DeferredEvent;
	RoleName = TEXT("마피아(대부)");
}

void UMafiaGodFatherRoleComponent::HandleResponseUseAbilityEvent(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	Super::HandleResponseUseAbilityEvent(InOther, InFlag, InEventType);
	if (InFlag == EMafiaUseAbilityFlag::Succeed)
	{
		EMafiaColor OtherColor = InOther->GetPlayerColor();		
		MAFIA_ULOG(LogMafiaAbility, Log, TEXT("%s을(를) 신비하게 만들도록 결정했습니다."), *GPlayerColorKoreanNames[int32(OtherColor)].ToString());
	}
}

void UMafiaGodFatherRoleComponent::HandleReceiveAffectedAbilityEvent(EMafiaRole InRole, AMafiaBasePlayerState* InOther)
{
	Super::HandleReceiveAffectedAbilityEvent(InRole, InOther);
}

void UMafiaGodFatherRoleComponent::HandleReceiveInstantEvent(AMafiaBasePlayerState* InOther)
{
	Super::HandleReceiveInstantEvent(InOther);
}

void UMafiaGodFatherRoleComponent::HandleNotifyResultAbilityEvent(AMafiaBasePlayerState* InOther)
{
	Super::HandleNotifyResultAbilityEvent(InOther);
}

void UMafiaGodFatherRoleComponent::HandleReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent)
{
	Super::HandleReceiveBroadCastEvent(InSender, InEvent);
}

void UMafiaGodFatherRoleComponent::HandleAffectedAbilityEvent(const FAffectedEvent& InEvent)
{
	Super::HandleAffectedAbilityEvent(InEvent);
}

void UMafiaGodFatherRoleComponent::HandleBroadCastEvent(const FBroadCastEvent& InEvent)
{
	Super::HandleBroadCastEvent(InEvent);
}

void UMafiaGodFatherRoleComponent::HandleStartVoteEvent()
{
	Super::HandleStartVoteEvent();
}

void UMafiaGodFatherRoleComponent::HandleResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
	Super::HandleResponseVoteEvent(InCandidate, InFlag);
}

void UMafiaGodFatherRoleComponent::HandleReceiveVoteResultEvent(AMafiaBasePlayerState* InDeathRow, EMafiaVoteResultFlag InFlag)
{
	Super::HandleReceiveVoteResultEvent(InDeathRow, InFlag);
}

void UMafiaGodFatherRoleComponent::HandleFinishVoteEvent()
{
	Super::HandleFinishVoteEvent();
}

void UMafiaGodFatherRoleComponent::OnRep_Dead()
{
}
