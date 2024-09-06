// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Player/MafiaBaseAbilityDwelling.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"

UMafiaBaseAbilityDwelling::UMafiaBaseAbilityDwelling(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


bool UMafiaBaseAbilityDwelling::Initialize(const EMafiaColor& InColorEnum, AMafiaBasePlayerState* InPlayerState)
{
	ResetAll();
	if (IsValid(InPlayerState))
	{
		if (UMafiaBaseRoleComponent* RoleComponent = InPlayerState->GetRoleComponent())
		{
			InPlayerState->ChangePlayerColor(InColorEnum);

			OriginPlayer = InPlayerState;
			ChangedPlayer = InPlayerState;

			bInitialized = true;

			return true;
		}
	}

	return false;
}

EMafiaUseAbilityFlag UMafiaBaseAbilityDwelling::DispatchInstantEvent(AMafiaBasePlayerState* InOther, EMafiaAbilityEventType InEventType)
{
	if (InEventType == EMafiaAbilityEventType::DeferredEvent)
	{
		return EMafiaUseAbilityFlag::Failed;
	}

	if (IsValid(InOther) == false)
	{
		return EMafiaUseAbilityFlag::Failed;
	}

	/** #Todo - ktw : 이벤트 즉시 처리. */
	if (OriginPlayer.IsValid())
	{
		if (UMafiaBaseRoleComponent* OriginPlayerRoleComponent = OriginPlayer.Get()->GetRoleComponent())
		{
			OriginPlayerRoleComponent->ReceiveInstantEvent(InOther);
			return EMafiaUseAbilityFlag::Succeed;
		}
	}
	return EMafiaUseAbilityFlag::Failed;
}

EMafiaUseAbilityFlag UMafiaBaseAbilityDwelling::AddDeferredAbilityEvent(AMafiaBasePlayerState* InOther, EMafiaAbilityEventType InEventType)
{
	if (InEventType == EMafiaAbilityEventType::InstantEvent)
	{
		return EMafiaUseAbilityFlag::Failed;
	}

	if (IsValid(InOther) == false)
	{
		return EMafiaUseAbilityFlag::Failed;
	}

	if (UMafiaBaseRoleComponent* OtherRoleComponent = InOther->GetRoleComponent())
	{
		EMafiaRole OtherRole = OtherRoleComponent->GetRoleType();

		FAbilityEvent Event;
		Event.AbilityUser = InOther;
		Event.bIgnoreChange = (OtherRole == EMafiaRole::Madam);

		DeferredEventArray.Add(Event);
		return EMafiaUseAbilityFlag::Succeed;
	}

	return EMafiaUseAbilityFlag::Failed;

	
}

bool UMafiaBaseAbilityDwelling::RemoveDeferredAbilityEvent(AMafiaBasePlayerState* InPlayerState, FAbilityEvent& OutRemovedEvent)
{
	if (IsValid(InPlayerState))
	{
		if (UMafiaBaseRoleComponent* RoleComponent = InPlayerState->GetRoleComponent())
		{
			EMafiaRole PlayerRole = RoleComponent->GetRoleType();
			FAbilityEvent* FindEvent = DeferredEventArray.FindByPredicate([&](const FAbilityEvent& Event)
			{
				if (Event.AbilityUser.IsValid())
				{
					if (UMafiaBaseRoleComponent* RoleComponent = Event.AbilityUser.Get()->GetRoleComponent())
					{
						return ((RoleComponent->GetRoleType()) == PlayerRole);
					}
				}
				return false;
					
			});

			if (FindEvent)
			{
				OutRemovedEvent = *FindEvent;

				DeferredEventArray.RemoveAll([&](const FAbilityEvent& Event)
				{
					if (Event.AbilityUser.IsValid())
					{
						if (UMafiaBaseRoleComponent* RoleComponent = Event.AbilityUser.Get()->GetRoleComponent())
						{
							return ((RoleComponent->GetRoleType()) == PlayerRole);
						}
					}
					return false;
				});

				return true;
			}
		}
	}
	

	return false;
}

void UMafiaBaseAbilityDwelling::StartAbilityEvent()
{
	ResetForNextRound();
}

void UMafiaBaseAbilityDwelling::PreBroadcastAbilityEvents()
{
	
}

void UMafiaBaseAbilityDwelling::BroadcastDeferredAbilityEvents()
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
				if (AMafiaBasePlayerState* AbilityUser = Event.AbilityUser.Get())
				{
					if (UMafiaBaseRoleComponent* AbilityUserComponent = AbilityUser->GetRoleComponent())
					{
						if (Event.bIgnoreChange)
						{
							OriginComponent->AffectedAbilityByOther(AbilityUserComponent->GetRoleType(), AbilityUser);
						}
						else
						{
							ChangedComponent->AffectedAbilityByOther(AbilityUserComponent->GetRoleType(), AbilityUser);
						}
					}
				}
			}


			for (auto& Event : DeferredEventArray)
			{
				if (AMafiaBasePlayerState* AbilityUser = Event.AbilityUser.Get())
				{
					if (UMafiaBaseRoleComponent* AbilityUserComponent = AbilityUser->GetRoleComponent())
					{
						if (Event.bIgnoreChange)
						{
							AbilityUserComponent->NotifyResultAbility(OriginPlayer.Get());
						}
						else
						{
							AbilityUserComponent->NotifyResultAbility(ChangedPlayer.Get());
						}
					}
				}
			}
		}
	}
	
	
}

bool UMafiaBaseAbilityDwelling::SendBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent)
{
	if (IsValid(InSender))
	{
		if (OriginPlayer.IsValid())
		{
			if (UMafiaBaseRoleComponent* OriginComponent = OriginPlayer.Get()->GetRoleComponent())
			{
				OriginComponent->ReceiveBroadCastEvent(InSender, InEvent);
				return true;
			}
		}
	}
	return false;
}

void UMafiaBaseAbilityDwelling::PostBroadcastAbilityEvents()
{
	
}

void UMafiaBaseAbilityDwelling::EndAbilityEvents()
{
	ResetForNextRound();
}



bool UMafiaBaseAbilityDwelling::SetChangedPlayer(AMafiaBasePlayerState* InAffectedPlayerState)
{
	ChangedPlayer = InAffectedPlayerState;
	InAffectedPlayerState->GetRoleComponent();

	return ChangedPlayer.IsValid();
}

void UMafiaBaseAbilityDwelling::ResetForNextRound()
{
	DeferredEventArray.Empty();
	ChangedPlayer = OriginPlayer;
}


void UMafiaBaseAbilityDwelling::ResetAll()
{
	bInitialized = false;

	DeferredEventArray.Empty();

	OriginPlayer = nullptr;
	ChangedPlayer = nullptr;
	
}
