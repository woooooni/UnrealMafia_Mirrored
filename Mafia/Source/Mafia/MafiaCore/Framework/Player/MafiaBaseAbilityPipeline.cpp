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
			ChangedPlayer = InPlayerState;

			bInitialized = true;

			return true;
		}
	}

	return false;
}

EMafiaUseAbilityFlag UMafiaBaseAbilityPipeline::DispatchInstantEvent(UMafiaBaseRoleComponent* InOther, EMafiaAbilityEventType InEventType)
{
	if (InEventType == EMafiaAbilityEventType::DeferredEvent)
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

EMafiaUseAbilityFlag UMafiaBaseAbilityPipeline::AddDeferredAbilityEvent(UMafiaBaseRoleComponent* InOther, 
	EMafiaAbilityEventType InEventType)
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
	Event.bIgnoreChange = (OtherRole == EMafiaRole::Madam);

	DeferredEventArray.Add(Event);
	return EMafiaUseAbilityFlag::Succeed;
}

bool UMafiaBaseAbilityPipeline::RemoveDeferredAbilityEvent(const EMafiaRole& InRole, FAbilityEvent& OutRemovedEvent)
{
	FAbilityEvent* FindEvent = DeferredEventArray.FindByPredicate([&](const FAbilityEvent& Event)
		{
			return ((Event.AbilityUser.Get()->GetRoleType()) == InRole);
		});

	if (FindEvent)
	{
		OutRemovedEvent = *FindEvent;
		DeferredEventArray.RemoveAll([&](const FAbilityEvent& Event)
			{
				return ((Event.AbilityUser.Get()->GetRoleType()) == InRole);
			});
		return true;
	}

	return false;
}

void UMafiaBaseAbilityPipeline::StartAbilityEvent()
{
	ResetForNextRound();
}

void UMafiaBaseAbilityPipeline::PreBroadcastAbilityEvents()
{

}

void UMafiaBaseAbilityPipeline::BroadcastDeferredAbilityEvent()
{
	/** 
		#Todo - ktw
			1. 이번 턴에 영향받을 플레이어에게 이벤트를 BroadCast.
			2. 나에게 능력을 사용한 Player에게 결과 통지.
	*/ 
	if (OriginPlayer.IsValid() && ChangedPlayer.IsValid())
	{
		UMafiaBaseRoleComponent* OriginComponent = OriginPlayer.Get()->GetRoleComponent();
		UMafiaBaseRoleComponent* ChangedComponent = ChangedPlayer.Get()->GetRoleComponent();

		if (IsValid(OriginComponent) && IsValid(ChangedComponent))
		{
			for (auto& Event : DeferredEventArray)
			{
				if (UMafiaBaseRoleComponent* AbilityUserComponent = Event.AbilityUser.Get())
				{
					if (Event.bIgnoreChange)
					{
						OriginComponent->AffectedAbilityByOther(AbilityUserComponent->GetRoleType(), AbilityUserComponent);
					}
					else
					{
						ChangedComponent->AffectedAbilityByOther(AbilityUserComponent->GetRoleType(), AbilityUserComponent);
					}
				}
			}


			for (auto& Event : DeferredEventArray)
			{
				if (UMafiaBaseRoleComponent* AbilityUserComponent = Event.AbilityUser.Get())
				{
					if (Event.bIgnoreChange)
					{
						AbilityUserComponent->NotifyResultAbility(OriginComponent);
					}
					else
					{
						AbilityUserComponent->NotifyResultAbility(ChangedComponent);
					}
				}
			}
		}
	}
	
	
}

void UMafiaBaseAbilityPipeline::PostBroadcastAbilityEvents()
{
	
}

void UMafiaBaseAbilityPipeline::EndAbilityEvents()
{
	ResetForNextRound();
}



bool UMafiaBaseAbilityPipeline::SetChangedPlayer(AMafiaBasePlayerState* InAffectedPlayerState)
{
	ChangedPlayer = InAffectedPlayerState;
	InAffectedPlayerState->GetRoleComponent();

	return ChangedPlayer.IsValid();
}

void UMafiaBaseAbilityPipeline::ResetForNextRound()
{
	DeferredEventArray.Empty();
	ChangedPlayer = OriginPlayer;
}


void UMafiaBaseAbilityPipeline::ResetAll()
{
	bInitialized = false;

	DeferredEventArray.Empty();

	OriginPlayer = nullptr;
	ChangedPlayer = nullptr;
	
}
