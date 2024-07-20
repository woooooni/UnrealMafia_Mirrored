// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Player/MafiaBasePlayerController.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/Character/MafiaBaseCharacter.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameMode.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/System/MafiaBaseGameInstance.h"
#include "Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "Framework/UI/Core/MafiaBaseHUD.h"
#include "Framework/Manager/MafiaChairManManager.h"
#include "Mafia/Framework/Character/MafiaSampleCharacter.h"
#include "Mafia.h"

AMafiaBasePlayerController::AMafiaBasePlayerController(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bEnableClickEvents = true;
	bShowMouseCursor = true;
}

void AMafiaBasePlayerController::PostInitializeComponents()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::PostInitializeComponents();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::InitPlayerState()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::InitPlayerState();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::SpawnDefaultHUD()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::SpawnDefaultHUD();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::PostNetInit()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::PostNetInit();
	BindDelegates();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::BeginPlay()
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::BeginPlay();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		TickDebug(DeltaSeconds);
	}
#endif
}

void AMafiaBasePlayerController::TickDebug(float DeltaSeconds)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UWorld* MyWorld = GetWorld();
	if (false == IsValid(MyWorld))
	{
		return;
	}

	AMafiaBaseGameState* GS = MyWorld->GetGameState<AMafiaBaseGameState>();
	if (false == IsValid(GS))
	{
		return;
	}

	AMafiaBasePlayerState* PS = GetPlayerState<AMafiaBasePlayerState>();
	if (false == IsValid(PS))
	{
		return;
	}

	int32 PopupKey = 123456;

	/** GameState:MatchState */
	{
		const FName& CurrentMatchState = GS->GetMatchState();
		const FString DebugString = FString::Printf(TEXT("MatchState:%s, FlowState:%s, Timer:%f"),
			*CurrentMatchState.ToString(), *EnumToStringMafia(EMafiaFlowState, GS->GetMafiaFlowState()), GS->GetGameFlowTimerRemaining());
		GEngine->AddOnScreenDebugMessage(PopupKey--, SMALL_NUMBER, FColor::Black, DebugString);
	}

	/** PlayerState:ReadyForGame */
	{
		const FString DebugString = FString::Printf(TEXT("bReady:%d"), PS->IsReadyForGame());
		GEngine->AddOnScreenDebugMessage(PopupKey--, SMALL_NUMBER, FColor::Black, DebugString);
	}

	/** PlayerState:Role */
	{
		int32 PlayerNum = 1;
		for (auto Pair : GS->GetJoinedUserPlayerStateMap())
		{
			AMafiaBasePlayerState* Val = Pair.Value.Get();
			if (IsValid(Val))
			{
				if (UMafiaBaseRoleComponent* RoleComponent = Val->GetRoleComponent())
				{
					if (PS->GetUniqueId() == Val->GetUniqueId())
					{
						const FString DebugString = FString::Printf(TEXT("My Role : %s"), *RoleComponent->GetRoleName().ToString());
						GEngine->AddOnScreenDebugMessage(PopupKey--, SMALL_NUMBER, FColor::Blue, DebugString);
					}
					else
					{
						const FString DebugString = FString::Printf(TEXT("Player %d Role : %s"), PlayerNum++, *RoleComponent->GetRoleName().ToString());
						GEngine->AddOnScreenDebugMessage(PopupKey--, SMALL_NUMBER, FColor::Yellow, DebugString);
					}
					
				}
				else
				{
					const FString DebugString = FString::Printf(TEXT("%s Role : %s"), *Val->GetUniqueId().ToString(), TEXT("NONE"));
					GEngine->AddOnScreenDebugMessage(PopupKey--, SMALL_NUMBER, FColor::Black, DebugString);
				}
			}
		}
		
	}
#endif
}

void AMafiaBasePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));
	Super::EndPlay(EndPlayReason);
	UnBindDelegates();
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

void AMafiaBasePlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Begin"));

	// Spawned HUD
	Super::ClientSetHUD_Implementation(NewHUDClass);

	if (AMafiaBaseHUD* HUD = Cast<AMafiaBaseHUD>(MyHUD))
	{
		HUD->InitializeHUD();
	}

	MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("End"));
}

#pragma region MatchCheat
void AMafiaBasePlayerController::CheatSetMinPlayerCount(const int32 InMinPlayerCount /*= 1*/)
{
#if ENABLE_CHEAT
	if (ROLE_AutonomousProxy == GetLocalRole())
	{
		ServerCheatSetMinPlayerCount(InMinPlayerCount);
	}
#endif
}

void AMafiaBasePlayerController::ServerCheatSetMinPlayerCount_Implementation(const int32 InMinPlayerCount)
{
	if (AMafiaBaseGameMode* MafiaBaseGameMode = GetWorld()->GetAuthGameMode<AMafiaBaseGameMode>())
	{
		MafiaBaseGameMode->SetGameStartMinPlayerCount(InMinPlayerCount);
	}
}

void AMafiaBasePlayerController::CheatSetReadyForGame(const bool bReady /*= true*/)
{
#if ENABLE_CHEAT
	if (ROLE_AutonomousProxy == GetLocalRole())
	{
		if (AMafiaBasePlayerState* MafiaBasePlayerState = GetPlayerState<AMafiaBasePlayerState>())
		{
			MafiaBasePlayerState->ServerReqReady(bReady);
		}
	}
#endif
}

void AMafiaBasePlayerController::CheatAssignAbility()
{
#if ENABLE_CHEAT
	if (GetLocalRole()== ROLE_AutonomousProxy)
	{
		ServerReqAssignAbility();
	}
	
#endif
}

void AMafiaBasePlayerController::CheatUseAbility(int32 InPlayerNum)
{
	if (AMafiaBasePlayerState* MyPlayerState = GetPlayerState<AMafiaBasePlayerState>())
	{
		UWorld* World = GetWorld();

		if (AMafiaBaseGameState* GS = World->GetGameState<AMafiaBaseGameState>())
		{
			if (0 >= InPlayerNum || GS->PlayerArray.IsValidIndex(InPlayerNum - 1) == false)
			{
				MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("Invalid Player Number"));
				return;
			}

			if (AMafiaBasePlayerState* OtherPlayerState = Cast<AMafiaBasePlayerState>(GS->PlayerArray[InPlayerNum - 1]))
			{
				if (MyPlayerState->GetUniqueId() != OtherPlayerState->GetUniqueId())
				{
					ServerReqUseAbility(MyPlayerState, OtherPlayerState);
				}
				else
				{
					MAFIA_ALOG(LogMafiaPlayerController, Warning, TEXT("That Player is My Player"));
				}
				
			}
		}
	}

	
}

void AMafiaBasePlayerController::CheatChangeCharacterColor(int32 InRed, int32 InGreen, int32 InBlue, int32 InAlpha)
{
	if (AMafiaSampleCharacter* DefaultCharacter = GetPawn<AMafiaSampleCharacter>())
	{
		DefaultCharacter->ChangeColor(FLinearColor(InRed, InGreen, InBlue, InAlpha), 0);
	}
}

void AMafiaBasePlayerController::ServerReqUseAbility_Implementation(AMafiaBasePlayerState* InMyPlayerState, AMafiaBasePlayerState* InOtherPlayerState)
{
	if (UMafiaBaseGameInstance* MafiaBaseGameInstance = UMafiaBaseGameInstance::Get(this))
	{
		if (MafiaBaseGameInstance->IsDedicatedServerInstance())
		{
			if (UMafiaChairManManager* MafiaChairManManager = MafiaBaseGameInstance->GetChairMan())
			{
				MafiaChairManManager->AddAbilityEvent(InMyPlayerState, InOtherPlayerState);
			}
		}
	}
}

void AMafiaBasePlayerController::ServerReqAssignAbility_Implementation()
{
	if (UMafiaBaseGameInstance* MafiaBaseGameInstance = UMafiaBaseGameInstance::Get(this))
	{
		if (MafiaBaseGameInstance->IsDedicatedServerInstance())
		{
			if (UMafiaChairManManager* MafiaChairManManager = MafiaBaseGameInstance->GetChairMan())
			{
				MafiaChairManManager->AssigningAbilities();
			}
		}
	}

}

#pragma endregion|