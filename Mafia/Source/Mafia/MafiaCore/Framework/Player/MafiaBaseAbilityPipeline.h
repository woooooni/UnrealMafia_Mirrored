// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaBaseAbilityPipeline.generated.h"




/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaBaseAbilityPipeline : public UObject
{	
	GENERATED_BODY()

protected:
	UMafiaBaseAbilityPipeline(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	UFUNCTION()
	bool Initialize(const EMafiaColor& InColorEnum, class AMafiaBasePlayerState* InPlayerState);

public:
	void BroadcastAffectedAbility();

public:
	bool SetAffectedPlayer(class AMafiaBasePlayerState* InAffectedPlayerState);

	FORCEINLINE class AMafiaBasePlayerState* GetOriginPlayerState() { return OriginPlayer.Get(); }
	FORCEINLINE class AMafiaBasePlayerState* GetAffectedPlayerState() { return AffectedPlayer.Get(); }


public:
	void ResetForNextRound();
	void ResetAll();

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<class UMafiaBaseRoleComponent>> AffectedEventsArray;

private:
	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> OriginPlayer;

	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> AffectedPlayer;

private:
	UPROPERTY()
	EMafiaColor DwellingColor;

	UPROPERTY()
	uint8 bInitialized : 1 = false;
};
