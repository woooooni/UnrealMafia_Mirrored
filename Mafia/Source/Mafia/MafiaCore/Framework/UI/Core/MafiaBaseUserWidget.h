// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MafiaBaseUserWidget.generated.h"

class AMafiaBaseHUD;
class AMafiaBaseCharacter;
class AMafiaBasePlayerController;

/**
 *
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class MAFIA_API UMafiaBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMafiaBaseUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//~ Begin UUserWidget Interface
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;
	//~ End UUserWidget Interface

	virtual void Finalize();

protected:
	template < class T >
	T* GetOwningHUD() const
	{
		if (APlayerController* MyPlayerController = GetOwningPlayer())
		{
			return MyPlayerController->GetHUD<T>();
		}

		return nullptr;
	}

protected:	
	UFUNCTION()
	virtual void OnStartAnimation();

	UFUNCTION()
	virtual void OnEndAnimation();

protected:
	virtual void OnAllWidgetsInitialized() {}
	virtual void BindDelegates() {}
	virtual void UnBindDelegates() {}

protected:
	UPROPERTY()
	uint8 bWidgetAnimationProgress : 1;

	FWidgetAnimationDynamicEvent WidgetAnimationStartDelegate;
	FWidgetAnimationDynamicEvent WidgetAnimationEndDelegate;

private:
	UPROPERTY(EditAnywhere, Category = "Widget Visibility Handle Config")
	bool bVisibleManageByState;

	
};
