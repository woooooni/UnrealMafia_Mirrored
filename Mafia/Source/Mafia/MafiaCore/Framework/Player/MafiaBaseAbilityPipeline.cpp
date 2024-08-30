// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Player/MafiaBaseAbilityPipeline.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"

UMafiaBaseAbilityPipeline::UMafiaBaseAbilityPipeline(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


bool UMafiaBaseAbilityPipeline::Initialize(const EMafiaColor& InColorEnum, AMafiaBasePlayerState* InPlayerState)
{
	ResetAll();
	if (IsValid(InPlayerState))
	{
		if (UMafiaBaseRoleComponent* RoleComponent = InPlayerState->GetRoleComponent())
		{
			InPlayerState->ChangePlayerColor(InColorEnum);

			PipelineColor = InColorEnum;

			OriginPlayer = InPlayerState;
			AffectedPlayer = InPlayerState;

			bInitialized = true;

			return true;
		}
	}

	return false;
}

EMafiaUseAbilityFlag UMafiaBaseAbilityPipeline::DispatchInstantEvent(UMafiaBaseRoleComponent* InOther, EMafiaAbilityEventType InEventType)
{
	if (InEventType == EMafiaAbilityEventType::DefferedEvent)
	{
		return EMafiaUseAbilityFlag::ImpossibleUseAbility;
	}

	if (IsValid(InOther) == false)
	{
		return EMafiaUseAbilityFlag::ImpossibleUseAbility;
	}

	/** #Todo - ktw : 이벤트 즉시 처리. */
	if (OriginPlayer.IsValid())
	{
		if (UMafiaBaseRoleComponent* OriginPlayerRoleComponent = OriginPlayer.Get()->GetRoleComponent())
		{
			OriginPlayerRoleComponent->RecieveInstantEvent(InOther);
			return EMafiaUseAbilityFlag::Succeed;
		}
	}
	return EMafiaUseAbilityFlag::ImpossibleUseAbility;
}

EMafiaUseAbilityFlag UMafiaBaseAbilityPipeline::AddDeferredAbilityEvent(UMafiaBaseRoleComponent* InOther, EMafiaAbilityEventType InEventType)
{
	if (InEventType == EMafiaAbilityEventType::InstantEvent)
	{
		return EMafiaUseAbilityFlag::ImpossibleUseAbility;
	}

	if (IsValid(InOther) == false)
	{
		return EMafiaUseAbilityFlag::ImpossibleUseAbility;
	}

	EMafiaRole OtherRole = InOther->GetRoleType();

	FAbilityEvent Event;
	Event.AbilityUser = InOther;
	Event.bIgnoreChange |= (OtherRole == EMafiaRole::Madam);

	DeferredEventsArray.Add(Event);
	return EMafiaUseAbilityFlag::Succeed;
}

void UMafiaBaseAbilityPipeline::BroadcastDeferredAbilityEvent()
{
	/** 
		#Todo - ktw
			1. 이번 턴에 영향받을 플레이어에게 이벤트를 BroadCast.
			2. 나에게 능력을 사용한 Player에게 결과 통지.
	*/ 
	if (AffectedPlayer.IsValid())
	{
		if (UMafiaBaseRoleComponent* AffectedComponent = AffectedPlayer.Get()->GetRoleComponent())
		{
			for (auto& Other : DeferredEventsArray)
			{
				if (UMafiaBaseRoleComponent* OtherComponent = Other.AbilityUser.Get())
				{
					AffectedComponent->AffectedAbilityByOther(OtherComponent->GetRoleType(), OtherComponent);
				}
			}

			for (auto& Other : DeferredEventsArray)
			{
				if (UMafiaBaseRoleComponent* OtherComponent = Other.AbilityUser.Get())
				{
					AffectedComponent->AffectedAbilityByOther(OtherComponent->GetRoleType(), OtherComponent);
				}
			}
		}
	}
	
	
}



bool UMafiaBaseAbilityPipeline::SetAffectedPlayer(AMafiaBasePlayerState* InAffectedPlayerState)
{
	AffectedPlayer = InAffectedPlayerState;
	InAffectedPlayerState->GetRoleComponent();

	return AffectedPlayer.IsValid();
}

void UMafiaBaseAbilityPipeline::ResetForNextRound()
{
	DeferredEventsArray.Empty();
	AffectedPlayer = OriginPlayer;
}


void UMafiaBaseAbilityPipeline::ResetAll()
{
	bInitialized = false;

	OriginPlayer = nullptr;
	AffectedPlayer = nullptr;
	
}
