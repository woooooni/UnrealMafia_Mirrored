// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaWorldDataManager.generated.h"

/**
 * AMafiaWorldSettings에서 SpawnActor로 생성됨
 */
UCLASS(Blueprintable, Transient)
class MAFIA_API UMafiaWorldDataManager final : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize();
};
