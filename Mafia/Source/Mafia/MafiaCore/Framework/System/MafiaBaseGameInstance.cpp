// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/System/MafiaBaseGameInstance.h"
#include "MafiaCore/Framework/System/MafiaGameEvents.h"
#include "MafiaCore/Framework/Manager/MafiaCheatManager.h"
#include "MafiaCore/Framework/GameModes/MafiaWorldSettings.h"
#include "Framework/Manager/MafiaChairManManager.h"
#include "Mafia.h"

#include "Kismet/GameplayStatics.h"

UMafiaBaseGameInstance* UMafiaBaseGameInstance::Get(const UObject* WorldContextObject)
{
	if (IsValid(WorldContextObject))
	{
		UMafiaBaseGameInstance* Instance = Cast<UMafiaBaseGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		return Instance;
	}
	return nullptr;
}

UMafiaWorldDataManager* UMafiaBaseGameInstance::GetMafiaWorldDataManager(const UObject* WorldContextObject)
{
	if (IsValid(WorldContextObject))
	{
		UWorld* World = WorldContextObject->GetWorld();
		if (IsValid(World))
		{
			AMafiaWorldSettings* MafiaWorldSettings = Cast<AMafiaWorldSettings>(World->GetWorldSettings());
			if (IsValid(MafiaWorldSettings))
			{
				return MafiaWorldSettings->GetMafiaWorldDataManager();
			}
		}
	}
	return nullptr;
}

void UMafiaBaseGameInstance::Init()
{
	/** @See UGameInstance::InitializeForPlayInEditor(int32 PIEInstanceIndex, const FGameInstancePIEParameters& Params) 에서 호출 */
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("Begin"));
	Super::Init();

	if (false == IsDedicatedServerInstance())
	{
		GameEvents = NewObject<UMafiaGameEvents>(this, UMafiaGameEvents::StaticClass());
		GEngine->OnNetworkFailure().AddUObject(this, &UMafiaBaseGameInstance::OnConnectionFailed);
	}
	else
	{
		MafiaChairManManager = NewObject<UMafiaChairmanManager>(this, UMafiaChairmanManager::StaticClass());
	}

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMafiaBaseGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMafiaBaseGameInstance::OnPostLoadMap);

	MafiaCheatManager = NewObject<UMafiaCheatManager>(this, UMafiaCheatManager::StaticClass());

	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("End"));
}

void UMafiaBaseGameInstance::Shutdown()
{
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("Begin"));
	if (IsValid(GameEvents))
	{
		GameEvents = nullptr;
	}

	if (IsValid(MafiaChairManManager))
	{
		MafiaChairManManager = nullptr;
	}

	Super::Shutdown();
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("End"));
}

void UMafiaBaseGameInstance::StartGameInstance()
{
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("Begin"));
	const bool ShouldMapLoad = false == FParse::Param(FCommandLine::Get(), TEXT("NoMapLoad"));
	MAFIA_ULOG(LogMafiaGameInstance, Log, TEXT("ShouldMapLoad = %d"), ShouldMapLoad);

	if (ShouldMapLoad)
	{
		Super::StartGameInstance();
	}
	else
	{
		MAFIA_ULOG(LogMafiaGameInstance, Error, TEXT("ShouldMapLoad = %d"), ShouldMapLoad);
		//OnStart();

		//// Engine Tick에서 GraphicContext가 없어서 생기는 크래시를 해결하기 위해 Flush를 호출함
		//ENQUEUE_RENDER_COMMAND(FlushCommand)(
		//	[](FRHICommandList& RHICmdList)
		//	{
		//		GRHICommandList.GetImmediateCommandList().ImmediateFlush(EImmediateFlushType::FlushRHIThread);
		//		RHIFlushResources();
		//		GRHICommandList.GetImmediateCommandList().ImmediateFlush(EImmediateFlushType::FlushRHIThreadFlushResources);
		//	});
		//FlushRenderingCommands();
	}
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("End"));
}

void UMafiaBaseGameInstance::OnStart()
{
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("Begin"));
	Super::OnStart();
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("End"));
}

bool UMafiaBaseGameInstance::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
#if ENABLE_CHEAT
	const TCHAR* Command = Cmd;
	FString OutString;
	if (FParse::Token(Command, OutString, true))
	{
		if (OutString == TEXT("sc.p"))
		{
			MafiaCheatManager->PlatFormCMD(Command);
			return true;
		}
	}

	if (MafiaCheatManager->ProcessConsoleExec(Cmd, Ar, Executor))
	{
		return true;
	}
#endif

	return Super::ProcessConsoleExec(Cmd, Ar, Executor);
}

UMafiaGameEvents* UMafiaBaseGameInstance::GetGameEvents() const
{
	return GameEvents;
}

#if WITH_EDITOR
FGameInstancePIEResult UMafiaBaseGameInstance::InitializeForPlayInEditor(int32 PIEInstanceIndex, const FGameInstancePIEParameters& Params)
{
	/** @See UGameInstance* UEditorEngine::CreateInnerProcessPIEGameInstance(FRequestPlaySessionParams& InParams, const FGameInstancePIEParameters& InPIEParameters, int32 InPIEInstanceIndex)에서 호출. 오직 에디터용 */
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("Called"));
	return Super::InitializeForPlayInEditor(PIEInstanceIndex, Params);
}

FGameInstancePIEResult UMafiaBaseGameInstance::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("Called"));
	return Super::StartPlayInEditorGameInstance(LocalPlayer, Params);
}
#endif

class AGameModeBase* UMafiaBaseGameInstance::CreateGameModeForURL(FURL InURL, UWorld* InWorld)
{
	/** @See bool UWorld::SetGameMode(const FURL& InURL)에서 호출 */
	/** 게임에서는 UEngine::LoadMap( FWorldContext& WorldContext, FURL URL, class UPendingNetGame* Pending, FString& Error ) 호출 */
	/** 에디터에서는 UGameInstance::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params) 호출 */
	MAFIA_ULOG(LogMafiaGameInstance, Warning, TEXT("Called"));
	return Super::CreateGameModeForURL(InURL, InWorld);
}

void UMafiaBaseGameInstance::OnPreLoadMap(const FString& PreloadMapPath)
{
	MAFIA_ULOG(LogMafiaGameInstance, Log, TEXT("Called OnPreLoadMap"));
}

void UMafiaBaseGameInstance::OnPostLoadMap(UWorld* InWorld)
{
	MAFIA_ULOG(LogMafiaGameInstance, Log, TEXT("Called OnPostLoadMap"));
}

void UMafiaBaseGameInstance::OnConnectionFailed(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	MAFIA_ULOG(LogMafiaGameInstance, Log, TEXT("Type : %s, ErrorString : %s"), ENetworkFailure::ToString(FailureType), *ErrorString);
}
