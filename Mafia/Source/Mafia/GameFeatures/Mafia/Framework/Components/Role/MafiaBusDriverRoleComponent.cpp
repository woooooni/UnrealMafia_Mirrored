// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Components/Role/MafiaBusDriverRoleComponent.h"

UMafiaBusDriverRoleComponent::UMafiaBusDriverRoleComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMafiaBusDriverRoleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMafiaBusDriverRoleComponent::BusDrive()
{
	Super::BusDrive();

}

void UMafiaBusDriverRoleComponent::OnRep_Dead()
{

}

void UMafiaBusDriverRoleComponent::ClientAffectedEventsFlush_Implementation()
{
}
