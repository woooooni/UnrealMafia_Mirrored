// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaBaseAbilityPipeline.generated.h"


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
	EMafiaUseAbilityFlag DispatchInstantEvent(class UMafiaBaseRoleComponent* InOther, EMafiaAbilityEventType InEventType);
	EMafiaUseAbilityFlag AddDeferredAbilityEvent(class UMafiaBaseRoleComponent* InOther, EMafiaAbilityEventType InEventType);
	void BroadcastDeferredAbilityEvent();

public:
	bool SetAffectedPlayer(class AMafiaBasePlayerState* InAffectedPlayerState);

	FORCEINLINE class AMafiaBasePlayerState* GetOriginPlayerState() { return OriginPlayer.Get(); }
	FORCEINLINE class AMafiaBasePlayerState* GetAffectedPlayerState() { return AffectedPlayer.Get(); }


	FORCEINLINE void SetDeferredEventArray(const TArray<FAbilityEvent>& InEventArray) { DeferredEventsArray = InEventArray; }
	FORCEINLINE const TArray<FAbilityEvent>& GetDeferredEventArray() const { return DeferredEventsArray; }


public:
	void ResetForNextRound();
	void ResetAll();

private:
	UPROPERTY()
	TArray<FAbilityEvent> DeferredEventsArray;

private:
	/** ktw : 원래 주인 플레이어 */
	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> OriginPlayer;

	/** ktw : 이번 턴에 영향 받는 플레이어 */
	UPROPERTY()
	TWeakObjectPtr<class AMafiaBasePlayerState> AffectedPlayer;

	UPROPERTY()
	EMafiaColor PipelineColor;

	UPROPERTY()
	uint8 bInitialized : 1 = false;
};
