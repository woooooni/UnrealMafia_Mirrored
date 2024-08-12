// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MafiaUITypes.generated.h"

class UMafiaBaseActionGroupWidget;

UENUM()
enum class EGameFlowEventsType : uint8
{
	Invalid,
	Lobby,
	Ready,
	InGame,
	EndedGame,
};

USTRUCT()
struct FWidgetVisibleList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> WidgetList;
};

/** 위젯에 대한 초기 옵션 설정 */
USTRUCT()
struct FMafiaWidgetConfig
{
	GENERATED_BODY()

public:
	FMafiaWidgetConfig()
		: bDefaultShow(false), ZOrder(0)
	{
	};

	UPROPERTY(EditDefaultsOnly, Category = "WidgetConfig")
	TSoftClassPtr<UMafiaBaseActionGroupWidget> WidgetClassReference;
	UPROPERTY(EditDefaultsOnly, Category = "WidgetConfig")
	bool bDefaultShow;
	UPROPERTY(EditDefaultsOnly, Category = "WidgetConfig")
	int ZOrder;
};

UCLASS(Abstract)
class MAFIA_API UMafiaUITypes : public UObject
{
	GENERATED_BODY()
	
};
