// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MafiaBasePlayerState.generated.h"

/**
 * AMafiaBasePlayerState는 BP없이 바로 생성하기 때문에 Abstract로 선언하면 안됨.
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class MAFIA_API AMafiaBasePlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	AMafiaBasePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	//~ Begin APlayerState Interface
	virtual void OnSetUniqueId() override;
	virtual void PostInitializeComponents() override;
	//~ End APlayerState Interface

public:
	UFUNCTION(Server, Reliable)
	void ServerReqReady(bool bReady);
	FORCEINLINE bool IsReadyForGame() const { return bReadyForGame; }
	
protected:
	/** 유저가 Ready버튼을 눌렀을때 */
	UPROPERTY(Replicated)
	uint8 bReadyForGame : 1;
};
