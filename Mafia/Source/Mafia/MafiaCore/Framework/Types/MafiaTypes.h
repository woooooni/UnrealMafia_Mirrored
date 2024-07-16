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

UENUM()
enum class EMafiaFlowState : uint8
{
	/** 메인 플로우 내에서 반복해서 변화되는 State */
	None,
	Day, // 아침
	Vote, // 투표
	Night, // 저녁
};

UENUM()
enum class EMafiaTeam : uint8
{
	Citizen,	/* 시민 */
	Mafia,		/* 마피아 */
	Neutral,	/* 중립 */
};


/** 
	ktw - 능력 처리 우선순위로  정렬되어 선언됨. 숫자가 낮을 수록, 이벤트 처리 우선순위가 높습니다. 
	UMafiaBaseRoleComponent::Heap
*/
UENUM()
enum class EMafiaRole : uint8
{
	Madam,			/* 마담 */

	Police,			/* 경찰 */

	Killer,			/* 연쇄살인마 */
	Mafia,			/* 마피아 */
	Vigilante,		/* 자경단원 */

	BusDriver,		/* 버스기사 */
	Detective,		/* 탐정 */
	Spirit,			/* 영매 */

	Thief,			/* 도둑 */
	
	Soldier,		/* 군인 */
	Doctor,			/* 의사 */
	
	Citizen,		/* 일반 시민 */
};

/** ktw - 게임 플레이 시민팀 필수 직업입니다. */
const static TArray<EMafiaRole> GCitizenEssentialRoleArray =
{ 
	EMafiaRole::Citizen,
	EMafiaRole::Police
};

/** ktw - 게임 플레이 시민팀 특수 직업입니다. */
const static TArray<EMafiaRole> GCitizenSpecialRoleArray =
{
	EMafiaRole::Doctor,
	EMafiaRole::Vigilante,
	EMafiaRole::Killer,
	EMafiaRole::BusDriver,
	EMafiaRole::Detective,
	EMafiaRole::Spirit,
	EMafiaRole::Soldier
};

/** ktw - 게임 플레이 마피아 팀 필수 직업입니다. */
const static TArray<EMafiaRole> GMafiaEssentialRoleArray =
{
	EMafiaRole::Mafia
};

/** ktw - 게임 플레이 마피아 팀 특수 직업입니다. */
const static TArray<EMafiaRole> GMafiaSpecialRoleArray =
{
	EMafiaRole::Madam,
	EMafiaRole::Thief
};


/** ktw - 인원수에 따른 마피아 팀 직업 부여 */
const static TArray<EMafiaRole> GMafiaAssignRoleArray =
{
	EMafiaRole::Mafia,
	EMafiaRole::Madam,
	EMafiaRole::Thief
};

/** ktw - 인원수에 따른 시민 팀 직업 부여 */
const static TArray<EMafiaRole> GCitizenAssignRoleArray =
{
	EMafiaRole::Police,
	EMafiaRole::Citizen,
	EMafiaRole::Doctor,
	EMafiaRole::Vigilante,
	EMafiaRole::Killer,
	EMafiaRole::BusDriver,
	EMafiaRole::Detective,
	EMafiaRole::Spirit,
	EMafiaRole::Soldier
};

const static TArray<EMafiaTeam> GTeamDistributionArray =
{
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Mafia,
	EMafiaTeam::Mafia,
	EMafiaTeam::Mafia,
	EMafiaTeam::Neutral,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
	EMafiaTeam::Citizen,
}



