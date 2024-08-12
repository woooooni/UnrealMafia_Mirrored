// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaCheatManager.generated.h"

/**
 * MafiaCheatManager는 C++ 클래스를 직접 생성하기 때문에 Abstract를 사용하면 안됨
 */
UCLASS(meta = (DisableNativeTick))
class MAFIA_API UMafiaCheatManager : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec)
	void CheatTemplate();

	// ServerCommand.Platform
	void PlatFormCMD(const FString& InCommand);
};
