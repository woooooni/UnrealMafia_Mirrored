// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/System/MafiaBaseGameInstance.h"
#include "MafiaGameInstance.generated.h"

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class MAFIA_API UMafiaGameInstance final : public UMafiaBaseGameInstance
{
	GENERATED_BODY()
	
private:
	virtual void Init() override;
	virtual void Shutdown() override;
};
