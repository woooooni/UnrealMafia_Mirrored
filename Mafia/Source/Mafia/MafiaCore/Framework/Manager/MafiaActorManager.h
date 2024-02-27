// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MafiaActorManager.generated.h"

UCLASS(Abstract, meta = (DisableNativeTick))
class MAFIA_API AMafiaActorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMafiaActorManager();

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
};
