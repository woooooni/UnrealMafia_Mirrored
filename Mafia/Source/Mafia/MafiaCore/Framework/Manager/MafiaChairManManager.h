// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaChairManManager.generated.h"


USTRUCT()
struct FUseAbilityEvent
{
	GENERATED_BODY()

public:
	EMafiaRole Role;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Origin;

	UPROPERTY()
	TWeakObjectPtr<class UMafiaBaseRoleComponent> Destination;

public:
	/** ktw - Pred Definition */
	FORCEINLINE bool operator< (const FUseAbilityEvent& Other) const
	{
		return Role > Other.Role;
	}
};

/**
 *	ktw - Server에 존재하는 GameInstance만 이 객체를 가지고 있습니다.
 */
UCLASS()
class MAFIA_API UMafiaChairManManager final : public UObject
{
	GENERATED_BODY()

public:
	UMafiaChairManManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION()
	void AssigningAbilities();

public:
	/** 
		ktw - Start -> Destination으로 향하는 능력 사용 이벤트를 Heap에 임시 저장합니다. 
		InOrigin : 능력 사용 Player, InDestination : 능력에 영향을 받은 Player.
	*/
	UFUNCTION()
	void AddAbilityEvent(class AMafiaBasePlayerState* InOrigin, class AMafiaBasePlayerState* InDestination);

	/** ktw - Heap에 저장된 이벤트들을 순회하면서 Flush합니다. */
	UFUNCTION()
	void FlushAbilityEvents();


protected:
	bool MakeShuffledRoleArray(int32 InPlayerCount, OUT TArray<EMafiaRole>& OutSuffledArray);

private:
	/* ktw - EMafiaRole에 선언된 Role의 값을 순서로 Event를 정렬합니다. */
	TArray<FUseAbilityEvent> CachedAbilityEventsHeap;
};
