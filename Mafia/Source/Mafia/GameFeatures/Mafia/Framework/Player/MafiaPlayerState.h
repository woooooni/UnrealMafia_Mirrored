// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaPlayerState.generated.h"

/**
 * final로 해놨는데, 추가로 상속하고 싶으면 지우고 작업하면 됨.
 */
UCLASS(meta = (DisableNativeTick))
class MAFIA_API AMafiaPlayerState final : public AMafiaBasePlayerState
{
	GENERATED_BODY()
	
public:
	AMafiaPlayerState(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE class UMafiaBaseRoleComponent* GetRoleComponent() { return RoleComponent; }


private:
	UPROPERTY()
	TObjectPtr<class UMafiaBaseRoleComponent> RoleComponent;
};
