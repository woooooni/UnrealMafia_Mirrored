// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Mafia.h"

AMafiaBasePlayerState::AMafiaBasePlayerState(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

}

void AMafiaBasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMafiaBasePlayerState, bReadyForGame);
}

void AMafiaBasePlayerState::BeginPlay()
{
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("Begin"));
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("End. GetOwner:%s"), GetOwner() ? *GetOwner()->GetName() : TEXT("Other Client"));
}

void AMafiaBasePlayerState::PostInitializeComponents()
{
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("Begin"));
	Super::PostInitializeComponents();
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("End"));
}

void AMafiaBasePlayerState::ServerReqReady_Implementation(bool bReady)
{
	bReadyForGame = bReady;
}

void AMafiaBasePlayerState::OnSetUniqueId()
{
	/**
	 * Called on both the client and server when unique ID has been modified
	 * GameMode의 Login에서 시작되어 AGameSession::RegisterPlayer에서 설정됨.
	 * 반드시 PostInitializeComponents가 먼저 호출된 후, OnSetUniqueId 호출된다.
	 *
	 * 클라의 GetWorld()->GetGameState의 설정은 MafiaBaseGameState::PostInitializeComponents에서 호출된다.
	 * ※ 클라에서 호출되는 타이밍에 GameState가 nullptr일 수 있다.
	 * PostInitializeComponents에서 PlayerArray가 등록됨.
	 */
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("Begin"));
	Super::OnSetUniqueId();

	if (nullptr == GetWorld()->GetGameState())
	{
		MAFIA_ALOG(LogMafiaPlayerState, Warning, TEXT("GetWorld()->GetGameState() is invalid, PlayerId:%d"), GetPlayerId());
	}

	/** 서버에서 추가되는 부분.클라에서도 타이밍 이슈로 여기서 추가될 수 있음. @see AMafiaBaseGameState::AddPlayerState(APlayerState * PlayerState) */
	if (AMafiaBaseGameState* GameState = GetWorld()->GetGameState<AMafiaBaseGameState>())
	{
		GameState->AddToPlayerStateMap(this);
	}
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("End"));
}