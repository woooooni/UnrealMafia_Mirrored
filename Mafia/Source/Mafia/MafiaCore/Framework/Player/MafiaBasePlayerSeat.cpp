// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Player/MafiaBasePlayerSeat.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"

UMafiaBasePlayerSeat::UMafiaBasePlayerSeat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


bool UMafiaBasePlayerSeat::SetOwner(AMafiaBasePlayerState* InPlayerState)
{
	OwnerPlayer = InPlayerState;
	OwnerRoleComponent = InPlayerState->GetRoleComponent();
	AffectedRoleComponent = OwnerRoleComponent.Get();

	return OwnerPlayer.IsValid() && OwnerRoleComponent.IsValid() && AffectedRoleComponent.IsValid();
}

void UMafiaBasePlayerSeat::RoundReset()
{
	AffectedRoleComponent = OwnerRoleComponent;
}


void UMafiaBasePlayerSeat::ResetAll()
{
	RoundReset();
	AffectedRoleComponent = nullptr;
	OwnerRoleComponent = nullptr;
	OwnerPlayer = nullptr;
}
