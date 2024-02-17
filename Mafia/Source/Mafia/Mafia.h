// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MafiaCore/Framework/System/MafiaLogChannels.h"
//#include "UObject/Class.h"
//#include "UObject/ObjectMacros.h"

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#define ENABLE_CHEAT 1
#else
#define ENABLE_CHEAT 0
#endif

#define DECLARE_FNAME(InName) static const FName Name(TEXT(#InName))

extern const FString GStringInvalidForEnum;
extern const FName GNameInvalidForEnum;

/** String을 통해 Enum의 특정 값을 가져오는 함수 - 매우 잘 사용하지 않음 */
template<typename TEnum>
inline FORCENOINLINE const TEnum GetStringToEnum(const FString& Str)
{
	FString EnumTypeName = GetEnumTypeName<TEnum>();
	static const UEnum* EnumPtr = FindFirstObject<UEnum>(*EnumTypeName, EFindFirstObjectOptions::ExactClass);
	if (!EnumPtr)
	{
		return TEnum();
	}

	return static_cast<TEnum>(EnumPtr->GetValueByNameString(Str));
}

/** 특정 Enum 타입을 FString으로 변환하는 함수 - 잘 사용하지 않음 */
template<typename TEnum>
inline FORCENOINLINE FString GetEnumTypeName()
{
	static_assert(TIsEnum<TEnum>::Value, "Template parameter TEnum should be an enumeration type.");
	const UEnum* EnumPtr = StaticEnum<TEnum>();
	FString EnumTypeName;
	if (EnumPtr)
	{
		EnumTypeName = EnumPtr->GetName();
	}
	return EnumTypeName;
}

template<typename TEnum>
inline FORCENOINLINE const FName GetEnumValueToName(const TCHAR* EnumName, TEnum Value)
{
	static const UEnum* EnumPtr = FindFirstObject<UEnum>(EnumName, EFindFirstObjectOptions::ExactClass);
	if (!EnumPtr)
	{
		return GNameInvalidForEnum;
	}

	return EnumPtr->GetNameByValue(int64(Value));
}

template<typename TEnum>
inline FORCENOINLINE const FString GetEnumValueToString(const TCHAR* EnumName, TEnum Value)
{
	// FORCENOINLINE 을 붙인 이유는 enumPtr caching이 좀더 효율적으로 되지 않을까 싶어서이다. (나중에 FORCENOINLINE 없이도 효율적인 것이 확인되면 지워도 된다.)
	// template 함수 안이라 이 cache가 각각의 enum type 별로 된다. 그래서 이렇게 cache해도 전혀 문제가 없다.
	static const UEnum* EnumPtr = FindFirstObject<UEnum>(EnumName, EFindFirstObjectOptions::ExactClass);
	if (!EnumPtr)
	{
		// return type 이 const & 라서 static으로 선언된 것을 리턴한다.
		return GStringInvalidForEnum;
	}

	return EnumPtr->GetNameStringByValue((int32)Value);
}

/** 특정 Enum에 대해 특정 Enum값에 해당하는 FString을 반환하는 함수 */
template<typename TEnum>
inline FORCENOINLINE const FString GetEnumValueToStringByValue(const TCHAR* Name, TEnum Value)
{
	const UEnum* EnumPtr = FindFirstObject<UEnum>(Name, EFindFirstObjectOptions::ExactClass);
	if (!EnumPtr)
	{
		return GStringInvalidForEnum;
	}

	return EnumPtr->GetNameStringByValue((int32)Value);
}

#define MAFIA_MESSAGE(Value) GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Cyan, FString(TEXT(#Value)));

#define EnumToNameMafia(EnumType, Value) GetEnumValueToName<EnumType>(TEXT(#EnumType), Value)
#define EnumToStringMafia(EnumType, Value)	GetEnumValueToString<EnumType>(TEXT(#EnumType), Value)
#define EnumToStringByValue(EnumType, Value) GetEnumValueToStringByValue<EnumType>(TEXT(#EnumType), Value)

#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? \
	*FString::Printf(TEXT("CLIENT:%d"), GetWorld()->GetOutermost()->GetPIEInstanceID()) : \
	((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))
#define LOG_LOCALROLEINFO *(GetEnumValueToString<ENetRole>(TEXT("ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(GetEnumValueToString<ENetRole>(TEXT("ENetRole"), GetRemoteRole()))
#define LOG_SUBLOCALROLEINFO *(GetEnumValueToString<ENetRole>(TEXT("ENetRole"), GetOwner()->GetLocalRole()))
#define LOG_SUBREMOTEROLEINFO *(GetEnumValueToString<ENetRole>(TEXT("ENetRole"), GetOwner()->GetRemoteRole()))
#define LOG_CALLINFO *(FString(TEXT("[") + FString(__FUNCTION__) + TEXT(":") + FString::FromInt(__LINE__) + TEXT("]")))

#define MAFIA_ULOG(LogCategory, Verbosity, Format, ...) UE_LOG(LogCategory, Verbosity, TEXT("%s %s"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define MAFIA_ALOG(LogCategory, Verbosity, Format, ...) UE_LOG(LogCategory, Verbosity, TEXT("[%s][%s/%s]%s %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define COLLISION_INTERACTION		ECC_GameTraceChannel1
#define COLLISION_CHARACTER			ECC_GameTraceChannel2
#define COLLISION_WORLD_COLLISION	ECC_GameTraceChannel3