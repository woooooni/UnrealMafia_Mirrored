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

			DwellingColor = InColorEnum;

			OriginPlayer = InPlayerState;
			AffectedPlayer = InPlayerState;

			bInitialized = true;

			return true;
		}
	}

	return false;
}

void UMafiaBaseAbilityPipeline::BroadcastAffectedAbility()
{
	for (auto& Weak : AffectedEventsArray)
	{
		if (UMafiaBaseRoleComponent* RoleComponent = Weak.Get())
		{
			// RoleComponent->AffectedAbilityByOther()
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
	AffectedEventsArray.Empty();
	AffectedPlayer = OriginPlayer;
}


void UMafiaBaseAbilityPipeline::ResetAll()
{
	bInitialized = false;

	OriginPlayer = nullptr;
	AffectedPlayer = nullptr;
	
}
