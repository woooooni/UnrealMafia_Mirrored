// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaBaseAbilityDwelling.generated.h"


enum class EDeferredEventKey
{
	Default,
	IgnoreChange,
};


USTRUCT()
struct FAbilityEvent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> AbilityUser;

	UPROPERTY()
	uint8 bIgnoreChange : 1 = false;
};

USTRUCT()
struct FAbilityEventArray
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FAbilityEvent> AbilityEventArray;
};


/**
 * 
 */
UCLASS()
class MAFIA_API UMafiaBaseAbilityDwelling : public UObject
{	
	GENERATED_BODY()

protected:
	UMafiaBaseAbilityDwelling(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	UFUNCTION()
	bool Initialize(const EMafiaColor& InColorEnum, class AMafiaBasePlayerState* InPlayerState);

public:
	EMafiaUseAbilityFlag DispatchInstantEvent(class UMafiaBaseRoleComponent* InOther, EMafiaAbilityEventType InEventType);
	EMafiaUseAbilityFlag AddDeferredAbilityEvent(class UMafiaBaseRoleComponent* InOther, EMafiaAbilityEventType InEventType);
	bool RemoveDeferredAbilityEvent(const EMafiaRole& InRole, FAbilityEvent& OutRemovedEvent);
	

public:
	void StartAbilityEvent();
	void PreBroadcastAbilityEvents();
	void BroadcastDeferredAbilityEvent();
	void PostBroadcastAbilityEvents();
	void EndAbilityEvents();

public:
	bool SetChangedPlayer(class AMafiaBasePlayerState* InChangePlayerState);

	FORCEINLINE class AMafiaBasePlayerState* GetOriginPlayerState() { return OriginPlayer.Get(); }
	FORCEINLINE class AMafiaBasePlayerState* GetChangedPlayerState() { return ChangedPlayer.Get(); }


	FORCEINLINE void SetDeferredEventArray(const TArray<FAbilityEvent>& InEventArray) { DeferredEventArray = InEventArray; }
	FORCEINLINE const TArray<FAbilityEvent>& GetDeferredEventArray() const { return DeferredEventArray; }


private:
	void ResetForNextRound();
	void ResetAll();

private:
	UPROPERTY()
	TArray<FAbilityEvent> DeferredEventArray;

private:
	/** ktw : 원래 주인 플레이어 */
	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> OriginPlayer;

	/** ktw : 이번 턴에 영향 받는 플레이어 */
	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> ChangedPlayer;

	UPROPERTY()
	EMafiaColor DwellingColor;

	UPROPERTY()
	uint8 bInitialized : 1 = false;
};
