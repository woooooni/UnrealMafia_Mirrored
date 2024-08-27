// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaBasePlayerSeat.generated.h"

/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaBasePlayerSeat : public UObject
{
	GENERATED_BODY()

protected:
	UMafiaBasePlayerSeat(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	UFUNCTION()
	bool Initialize(const FLinearColor& InSeatColor, class AMafiaBasePlayerState* InPlayerState);

public:
	FORCEINLINE class AMafiaBasePlayerState* GetOriginPlayerState() { return OriginPlayer.Get(); }
	FORCEINLINE class UMafiaBaseRoleComponent* GetOriginRoleComponent() { return OriginRoleComponent.Get(); }
	FORCEINLINE class UMafiaBaseRoleComponent* GetAffectedRoleComponent() { return AffectedRoleComponent.Get(); }
	

public:
	void ResetForNextRound();
	void ResetAll();

private:
	UPROPERTY()
	FLinearColor SeatColor;

	UPROPERTY()
	uint8 bInitialized : 1 = false;

	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> OriginPlayer;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> OriginRoleComponent;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> AffectedRoleComponent;

};
