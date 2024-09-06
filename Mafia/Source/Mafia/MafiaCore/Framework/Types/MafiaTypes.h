// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "MafiaTypes.generated.h"

/**
 * 
 */

namespace MafiaMatchState
{
	/** WaitingPlayers: 로비에서 유저를 기다리고 Ready를 기다리는 단계 */
	//extern MAFIA_API const FName WaitingPlayers;
	/** AssigningAbility: 게임이 시작되어 직업을 선택하는 단계 */
	extern MAFIA_API const FName AssigningAbility;
	/** InProgressMafia: 직업 선택후 마피아 게임을 시작하는 단계 */
	extern MAFIA_API const FName InProgressMafia;
	/** WaitingPostRound: 마피아 게임이 종료되어 다음 라운드로 넘어가는 단계 */
	extern MAFIA_API const FName WaitingPostRound;
}

UENUM(BlueprintType)
enum class EMafiaColor : uint8
{
	Red,
	Blue,
	Green,
	Yellow,
	Orange,
	Purple,
	Brown,
	Gray,
	Black,
	SkyBlue,
	LimeGreen,
	Teal,
	Beige,
	Navy,
	Mint,
	Lavender,
};

const static TArray<FLinearColor> GPlayerColors = {
	FLinearColor(1.0f, 0.0f, 0.0f),  // Red
	FLinearColor(0.0f, 0.0f, 1.0f),  // Blue
	FLinearColor(0.0f, 1.0f, 0.0f),  // Green
	FLinearColor(1.0f, 1.0f, 0.0f),  // Yellow
	FLinearColor(1.0f, 0.5f, 0.0f),  // Orange
	FLinearColor(0.5f, 0.0f, 0.5f),  // Purple
	FLinearColor(0.6f, 0.3f, 0.0f),  // Brown
	FLinearColor(0.5f, 0.5f, 0.5f),  // Gray
	FLinearColor(0.0f, 0.0f, 0.0f),  // Black
	FLinearColor(0.53f, 0.81f, 0.92f), // Sky Blue
	FLinearColor(0.7f, 1.0f, 0.0f),  // Lime Green
	FLinearColor(0.0f, 0.5f, 0.5f),  // Teal
	FLinearColor(0.96f, 0.96f, 0.86f), // Beige
	FLinearColor(0.0f, 0.0f, 0.5f),  // Navy
	FLinearColor(0.74f, 1.0f, 0.78f),  // Mint
	FLinearColor(0.9f, 0.9f, 0.98f)   // Lavender
};

const static TArray<FName> GPlayerColorEnglishNames =
{
	TEXT("Red"),
	TEXT("Blue"),
	TEXT("Green"),
	TEXT("Yello"),
	TEXT("Orange"),
	TEXT("Purple"),
	TEXT("Brown"),
	TEXT("Gray"),
	TEXT("Black"),
	TEXT("SkyBlue"),
	TEXT("LimeGreen"),
	TEXT("Teal"),
	TEXT("Beige"),
	TEXT("Navy"),
	TEXT("Mint"),
	TEXT("Lavender"),
};


const static TArray<FName> GPlayerColorKoreanNames =
{
	TEXT("빨강"),
	TEXT("파랑"),
	TEXT("초록"),
	TEXT("노랑"),
	TEXT("주황"),
	TEXT("보라"),
	TEXT("갈색"),
	TEXT("회색"),
	TEXT("검정"),
	TEXT("연파"),
	TEXT("연초"),
	TEXT("청록"),
	TEXT("베이지"),
	TEXT("남색"),
	TEXT("민트"),
	TEXT("연보"),
};



UENUM()
enum class EMafiaFlowState : uint8
{
	/** 메인 플로우 내에서 반복해서 변화되는 State */
	None,

	BeforeDay,		// 아침 이전.
	Day,			// 아침
	AfterDay,		// 아침 이후
	EndDay,			// 아침 -> 투표 이전.

	BeforeVote,		// 투표 이전
	Vote,			// 투표.
	AfterVote,		// 투표 이후
	EndVote,		// 투표 -> 처형 이전 or 저녁 이전.

	BeginPunishment,	// 처형 이전.
	Punishment,			// 처형 중
	AfterPunishment,	// 처형 이후
	EndPunishment,			// 처형 -> 저녁 이전

	BeforeNight,	// 저녁 시작 전
	Night,			// 저녁
	AfterNight,		// 저녁 이후
	EndNight,		// 저녁 -> 아침 이전.


	GameOver,		// 게임 오버
};

UENUM()
enum class EMafiaTeam : uint8
{
	Citizen,	/* 시민 */
	Mafia,		/* 마피아 */
	Neutral,	/* 중립 */
};

UENUM()
enum class EMafiaGameResult
{
	None,
	CitizenWin,
	MafiaWin,
	NeutralWin,
	Invalid,
};


/** 
	ktw - 능력 처리 우선순위로  정렬되어 선언됨. 숫자가 낮을 수록, 이벤트 처리 우선순위가 높습니다. 
	UMafiaBaseRoleComponent::Heap
*/
UENUM()
enum class EMafiaRole : uint8
{
	Madam,			/* 기생, 마담 */

	BusDriver,		/* 버스기사 */

	Police,			/* 경찰 */
	Killer,			/* 연쇄살인마 */
	GodFather,			/* 마피아 */
	Vigilante,		/* 자경단원 */

	Detective,		/* 탐정 */
	Spirit,			/* 영매 */

	Thief,			/* 도둑 */
	
	Soldier,		/* 군인 */
	Doctor,			/* 의사 */
	
	Citizen,		/* 일반 시민 */
};

UENUM()
enum class EMafiaAbilityEventType : uint8 
{
	None,
	InstantEvent,
	DeferredEvent,
	BusDriveEvent,
};

UENUM()
enum class EMafiaUseAbilityFlag : uint8
{
	ImpossibleUseAbility,
	AlreadyUseAbility,
	Failed,
	Succeed,
};

UENUM()
enum class EMafiaBroadCastEventFlag : uint8
{
	Failed,
	Succeed,
};

UENUM()
enum class EMafiaBroadCastEvent : uint8
{
	/** ktw : 모든 플레이어가 관람하는 전체 이벤트를 정의합니다. */

	TrafficDead,	// 교통사고 - 버스기사의 승객이 다른 승객을 죽임.
	Dead,			// 이번 턴에 죽음.
	DoctorHeal,		// 의사가 살림.
	DoctorHealDead,	// 의사가 살렸으나, 죽음.
};


UENUM()
enum class EMafiaVoteFlag : uint8
{
	ImpossibleVote,
	AlreadyVoted,
	Succeed,
};

UENUM()
enum class EMafiaVoteResultFlag : uint8
{
	NoDeathPlayer,
	SomeoneDying,
};



#pragma region Deprecated.
///** ktw - 게임 플레이 시민팀 필수 직업입니다. */
//const static TArray<EMafiaRole> GCitizenEssentialRoleArray =
//{ 
//	EMafiaRole::Citizen,
//	EMafiaRole::Police
//};
//
///** ktw - 게임 플레이 시민팀 특수 직업입니다. */
//const static TArray<EMafiaRole> GCitizenSpecialRoleArray =
//{
//	EMafiaRole::Doctor,
//	EMafiaRole::Vigilante,
//	EMafiaRole::Killer,
//	EMafiaRole::BusDriver,
//	EMafiaRole::Detective,
//	EMafiaRole::Spirit,
//	EMafiaRole::Soldier
//};
//
///** ktw - 게임 플레이 마피아 팀 필수 직업입니다. */
//const static TArray<EMafiaRole> GMafiaEssentialRoleArray =
//{
//	EMafiaRole::Mafia
//};
//
///** ktw - 게임 플레이 마피아 팀 특수 직업입니다. */
//const static TArray<EMafiaRole> GMafiaSpecialRoleArray =
//{
//	EMafiaRole::Madam,
//	EMafiaRole::Thief
//};
#pragma endregion

/** ktw - 인원수에 따른 마피아 팀 직업 부여 */
const static TArray<EMafiaRole> GMafiaAssignRoleArray =
{
	EMafiaRole::GodFather,
	EMafiaRole::Madam,
	EMafiaRole::Thief
};

/** ktw - 인원수에 따른 시민 팀 직업 부여 */
const static TArray<EMafiaRole> GCitizenAssignRoleArray =
{
	EMafiaRole::Police,
	EMafiaRole::Citizen,
	EMafiaRole::Doctor,
	EMafiaRole::Soldier,
	EMafiaRole::BusDriver,
	EMafiaRole::Vigilante,
	EMafiaRole::Killer,
	EMafiaRole::Detective,
	EMafiaRole::Spirit,
};

const static TArray<EMafiaTeam> GTeamDistributionArray =
{
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Mafia,
	EMafiaTeam::Neutral,
	EMafiaTeam::Mafia,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Mafia,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
};