// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MafiaCore/Framework/Types/MafiaTypes.h"
#include "MafiaCore/Framework/Character/MafiaBaseCharacter.h"
#include "Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "MafiaGameEvents.generated.h"

/**
 * 인게임에서 벌어지는 이벤트를 모두 한곳에서 관리하기 위한 클래스
 * 델리게이트 선언과 정의를 해주면 된다. C++에서 직접 동적할당 하기 때문에 Abstract해서는 안된다
 */
UCLASS(meta = (DisableNativeTick))
class MAFIA_API UMafiaGameEvents : public UObject
{
	GENERATED_BODY()
	
public:
	/** 이곳에 델리게이트 선언과 정의를 해주면 됨 */

	/** 모든 위젯이 생성된 이후에 호출되는 타이밍을 알고 싶을때 사용 */
	DECLARE_MULTICAST_DELEGATE(FOnInitializedAllWidgets);
	FOnInitializedAllWidgets OnInitializedAllWidgets;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedMatchState, const FName&);
	FOnChangedMatchState OnChangedMatchState;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedMafiaFlowState, const EMafiaFlowState&);
	FOnChangedMafiaFlowState OnChangedMafiaFlowState;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangedPlayerName, const AMafiaBaseCharacter* InCharacter, const FName& InPlayerNewName);
	FOnChangedPlayerName OnChangedPlayerName;

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnResponseUseAbility, const AMafiaBasePlayerState* InOther, const EMafiaUseAbilityFlag InFlag, const EMafiaAbilityEventType InEventType);
	FOnResponseUseAbility OnResponseUseAbility;

	DECLARE_MULTICAST_DELEGATE(FOnClickedPlayerCard)
	FOnClickedPlayerCard OnClickedPlayerCard;
};


/**
 * 밑의 코드들은 Core한 내용이기 때문에
 * 특별한 사유가 아니면 절대 수정하지 마세요 - SungHoon
 */

#pragma region CoreCode
UCLASS(Abstract, meta = (DisableNativeTick))
class UMafiaGameEventsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Singleton", meta = (WorldContext = "WorldContextObject"))
	static UMafiaGameEvents* GetGameEvents(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Singleton", meta = (WorldContext = "WorldContextObject"))
	static bool IsGameEventsValid(const UObject* WorldContextObject);

	//UFUNCTION(BlueprintCallable, Category = "Singleton", meta = (WorldContext = "WorldContextObject"))
	//static bool CheckSendGameEvent(UObject* Object, ECheckCondition Codition = ECheckCondition::HasController_NoSpectating);

	//UFUNCTION(BlueprintCallable, Category = "Singleton", meta = (WorldContext = "WorldContextObject"))
	//static void NotifyDisplayMessage(const UObject* WorldContextObject, EMetaUIModelessPopupDataName PopupID);

	//UFUNCTION(BlueprintCallable, Category = "Singleton", meta = (WorldContext = "WorldContextObject"))
	//static void NotifyDisplayMessageWithStr(const UObject* WorldContextObject, EMetaUIModelessPopupDataName PopupID, const FString& AdditionalString);

	//UFUNCTION(BlueprintCallable, Category = "Singleton", meta = (WorldContext = "WorldContextObject"))
	//static void NotifyDisplayRawMessage(const UObject* WorldContextObject, FString& MessageString, const float ExposureTime);

	//NOTE(CS) : 맴버 함수가 const 함수가 아닐때 사용
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	static FDelegateHandle BindMulticastDelegate(const UObject* WorldContextObject, TMulticastDelegate<void(ParamTypes...)>& Delegate, UserClass* InUserObject, typename TMemFunPtrType<false, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc, VarTypes... Vars);

	//NOTE(CS) : 맴버 함수가 const 함수일 때 사용
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	static FDelegateHandle BindMulticastDelegate(const UObject* WorldContextObject, TMulticastDelegate<void(ParamTypes...)>& Delegate, UserClass* InUserObject, typename TMemFunPtrType<true, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc, VarTypes... Vars);

	template <typename... ParamTypes>
	static bool UnbindMulticastDelegate(const UObject* WorldContextObject, TMulticastDelegate<void(ParamTypes...)>& Delegate, FDelegateHandle DelegateHandle);
};

template <typename UserClass, typename... ParamTypes, typename... VarTypes>
FDelegateHandle UMafiaGameEventsLibrary::BindMulticastDelegate(const UObject* WorldContextObject, TMulticastDelegate<void(ParamTypes...)>& Delegate, UserClass* InUserObject, typename TMemFunPtrType<true, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc, VarTypes... Vars)
{
	FDelegateHandle Handle;
	if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject))
	{
		Handle = Delegate.AddUObject(InUserObject, InFunc, Vars...);
	}
	return Handle;
}

template <typename UserClass, typename... ParamTypes, typename... VarTypes>
FDelegateHandle UMafiaGameEventsLibrary::BindMulticastDelegate(const UObject* WorldContextObject, TMulticastDelegate<void(ParamTypes...)>& Delegate, UserClass* InUserObject, typename TMemFunPtrType<false, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc, VarTypes... Vars)
{
	FDelegateHandle Handle;
	if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject))
	{
		Handle = Delegate.AddUObject(InUserObject, InFunc, Vars...);
	}
	return Handle;
}

template <typename... ParamTypes>
bool UMafiaGameEventsLibrary::UnbindMulticastDelegate(const UObject* WorldContextObject, TMulticastDelegate<void(ParamTypes...)>& Delegate, FDelegateHandle DelegateHandle)
{
	bool RemovalResult = false;
	if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject))
	{
		RemovalResult = Delegate.Remove(DelegateHandle);
	}
	return RemovalResult;
}

#define BindGameEvent(DelegateName, FuncName) \
    UMafiaGameEventsLibrary::BindMulticastDelegate(GetWorld(), UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName, this, FuncName);

#define UnbindGameEvent(DelegateName, DelegateHandle) \
    UMafiaGameEventsLibrary::UnbindMulticastDelegate(GetWorld(), UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName, DelegateHandle);    

#define SendGameEvent(DelegateName) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(); \
		} \
	}

#define SendGameEvent_OneParam(DelegateName, Param1) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1); \
		} \
	}

#define SendGameEvent_TwoParams(DelegateName, Param1, Param2) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1, Param2); \
		} \
	}

#define SendGameEvent_ThreeParams(DelegateName, Param1, Param2, Param3) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3); \
		} \
	}

#define SendGameEvent_FourParams(DelegateName, Param1, Param2, Param3, Param4) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4); \
		} \
	}

#define SendGameEvent_FiveParams(DelegateName, Param1, Param2, Param3, Param4, Param5) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5); \
		} \
	}

#define SendGameEvent_SixParams(DelegateName, Param1, Param2, Param3, Param4, Param5, Param6) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5, Param6); \
		} \
	}

#define SendGameEvent_SevenParams(DelegateName, Param1, Param2, Param3, Param4, Param5, Param6, Param7) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5, Param6, Param7); \
		} \
	}

#define SendGameEvent_EightParams(DelegateName, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8); \
		} \
	}

#define SendGameEvent_NineParams(DelegateName, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9) \
	if (UMafiaGameEventsLibrary::IsGameEventsValid(GetWorld())) \
	{ \
		if(UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.IsBound()) \
		{ \
			UMafiaGameEventsLibrary::GetGameEvents(GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9); \
		} \
	}


// SendGameEvent_WithContext
#define SendGameEvent_InContext(DelegateName, WorldContextObject) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(); \
			} \
		} \
	}

#define SendGameEvent_InContext_OneParam(DelegateName, WorldContextObject, Param1) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1); \
			} \
		} \
	}

#define SendGameEvent_InContext_TwoParams(DelegateName, WorldContextObject, Param1, Param2) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1, Param2); \
			} \
		} \
	}

#define SendGameEvent_InContext_ThreeParams(DelegateName, WorldContextObject, Param1, Param2, Param3) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3); \
			} \
		} \
	}

#define SendGameEvent_InContext_FourParams(DelegateName, WorldContextObject, Param1, Param2, Param3, Param4) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4); \
			} \
		} \
	}

#define SendGameEvent_InContext_FiveParams(DelegateName, WorldContextObject, Param1, Param2, Param3, Param4, Param5) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5); \
			} \
		} \
	}

#define SendGameEvent_InContext_SixParams(DelegateName, WorldContextObject, Param1, Param2, Param3, Param4, Param5, Param6) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5, Param6); \
			} \
		} \
	}

#define SendGameEvent_InContext_SevenParams(DelegateName, WorldContextObject, Param1, Param2, Param3, Param4, Param5, Param6, Param7) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5, Param6, Param7); \
			} \
		} \
	}

#define SendGameEvent_InContext_EightParams(DelegateName, WorldContextObject, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8); \
			} \
		} \
	}

#define SendGameEvent_InContext_NineParams(DelegateName, WorldContextObject, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9) \
	if(IsValid(WorldContextObject)) \
	{ \
		if (UMafiaGameEventsLibrary::IsGameEventsValid(WorldContextObject->GetWorld())) \
		{ \
			if(UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.IsBound()) \
			{ \
				UMafiaGameEventsLibrary::GetGameEvents(WorldContextObject->GetWorld())->DelegateName.Broadcast(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9); \
			} \
		} \
	}
#pragma endregion CoreCode