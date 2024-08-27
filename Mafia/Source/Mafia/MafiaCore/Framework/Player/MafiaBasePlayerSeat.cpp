// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Player/MafiaBasePlayerSeat.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"

UMafiaBasePlayerSeat::UMafiaBasePlayerSeat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


bool UMafiaBasePlayerSeat::Initialize(const FLinearColor& InSeatColor, AMafiaBasePlayerState* InPlayerState)
{
	ResetAll();
	if (IsValid(InPlayerState))
	{
		if (UMafiaBaseRoleComponent* RoleComponent = InPlayerState->GetRoleComponent())
		{
			InPlayerState->ChangePlayerColor(InSeatColor);

			bInitialized = true;
			OriginPlayer = InPlayerState;
			OriginRoleComponent = RoleComponent;
			AffectedRoleComponent = RoleComponent;

			return true;
		}
	}
	
	return false;
}


void UMafiaBasePlayerSeat::ResetForNextRound()
{
	AffectedRoleComponent = OriginRoleComponent;
}


void UMafiaBasePlayerSeat::ResetAll()
{
	bInitialized = false;
	OriginPlayer = nullptr;
	OriginRoleComponent = nullptr;
	AffectedRoleComponent = nullptr;
}
