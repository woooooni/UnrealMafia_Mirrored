// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Mafia.h"
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

private:
	UFUNCTION(Client, Reliable)
	void PostInitializeRoleComponent();

public:
	FORCEINLINE class UMafiaBaseRoleComponent* GetRoleComponent() { return RoleComponent; }

#pragma region Cheat
public:
	UFUNCTION(Server, Reliable)
	void CheatSetRole(EMafiaRole InRole);
#pragma endregion Cheat

public:
	/** ktw - 서버에서 호출해야 합니다. */
	UFUNCTION()
	bool AssignAbility(EMafiaRole InRole);


public:

	/** ktw - 클라이언트에서 호출해야 합니다. */
	UFUNCTION()
	void ChangePlayerColor(FLinearColor InColor);
	FORCEINLINE FLinearColor GetPlayerColor() { return PlayerColor; }

protected:
	/** ktw : 플레이어 Role Component 생성. AssignAbility에서 호출됩니다. */
	UFUNCTION()
	void CreateRoleComponent(EMafiaRole InRole);

protected:
	UFUNCTION(Server, UnReliable)
	void ServerChangePlayerColor(FLinearColor InColor);

	
protected:
	/** 유저가 Ready버튼을 눌렀을때 */
	UPROPERTY(Replicated)
	uint8 bReadyForGame : 1;

	/** Player Color */
	UPROPERTY(Replicated)
	FLinearColor PlayerColor;

	/** Mafia Role */
	UPROPERTY(Replicated)
	TObjectPtr<class UMafiaBaseRoleComponent> RoleComponent;


};
