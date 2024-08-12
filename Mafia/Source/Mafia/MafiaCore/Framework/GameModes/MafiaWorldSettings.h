// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "MafiaWorldSettings.generated.h"

class UMafiaWorldDataManager;
/**
 * 사용법 : Cast<AMafiaWorldSettings>(GetWorld->GetWorldSettings());
 * 일반적으로는 WorldSetting에서 매니저 객체를 가지지 않는다. 용도가 다름.
 * 몇가지 기본 셋팅값 변수를 두기 위함이지만, 연습삼아 이곳에 매니저 클래스를 둬보았다.
 * chang님의 WorldDataMaanger를 연습해봄. World 엔진 코드에서 넣으셨음.
 */
UCLASS(meta = (DisableNativeTick))
class MAFIA_API AMafiaWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	FORCEINLINE UMafiaWorldDataManager* GetMafiaWorldDataManager() const { return MafiaWorldDataManager; }

protected:
	explicit AMafiaWorldSettings(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(EditAnywhere, Category = Manager)
	TSubclassOf<UMafiaWorldDataManager> MafiaWorldDataManagerClass;
	UPROPERTY(Transient)
	TObjectPtr<UMafiaWorldDataManager> MafiaWorldDataManager;
};
