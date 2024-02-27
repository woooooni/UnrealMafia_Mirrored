// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Character/MafiaBaseCharacter.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "Mafia.h"
#include "Net/UnrealNetwork.h"

AMafiaBaseCharacter::AMafiaBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMafiaBaseCharacter::BeginPlay()
{
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("Begin"));
	Super::BeginPlay();
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("End"));
}

void AMafiaBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMafiaBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMafiaBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("Begin"));
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("End"));
}

void AMafiaBaseCharacter::OnRep_Owner()
{
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("Begin"));
	Super::OnRep_Owner();
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("End"));
}

void AMafiaBaseCharacter::OnRep_Controller()
{
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("Begin"));
	Super::OnRep_Controller();
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("End"));
}

void AMafiaBaseCharacter::OnRep_PlayerState()
{
	/**
	 * 이 함수는 Simulated & AutonomusProxy에서 모두 호출되는 함수
	 */
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("Begin"));
	Super::OnRep_PlayerState();
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("End"));
}

void AMafiaBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("Begin"));
	Super::EndPlay(EndPlayReason);
	UnBindDelegates();
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("End"));
}

void AMafiaBaseCharacter::PostInitializeComponents()
{
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("Begin"));
	Super::PostInitializeComponents();
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("End"));
}

void AMafiaBaseCharacter::PostNetInit()
{
	/** 최초 생성시, 리플리케이트가 모두 완료된 후 호출됨. PostInitializeComponents 이후에 호출. AutonomusProxy 에서만 호출 */
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("Begin"));
	Super::PostNetInit();
	MAFIA_ALOG(LogMafiaCharacter, Warning, TEXT("End"));
}

void AMafiaBaseCharacter::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Clicked")));
#endif
}

void AMafiaBaseCharacter::NotifyActorOnReleased(FKey ButtonReleased /*= EKeys::LeftMouseButton*/)
{
	/** 주의할점 : 마지막에 릴리즈될때 위치가 Actor에 있어야 호출됨 */
	Super::NotifyActorOnReleased(ButtonReleased);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Released")));
#endif
}