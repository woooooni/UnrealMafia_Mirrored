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
	bool SetOwner(class AMafiaBasePlayerState* InPlayerState);

	FORCEINLINE class UMafiaBaseRoleComponent* GetAffectedRoleComponent() { return AffectedRoleComponent.Get(); }
	FORCEINLINE class UMafiaBaseRoleComponent* GetOwnerRoleComponent() { return AffectedRoleComponent.Get(); }



public:
	void RoundReset();
	void ResetAll();


private:

	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> OwnerPlayer;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> OwnerRoleComponent;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> AffectedRoleComponent;

};
