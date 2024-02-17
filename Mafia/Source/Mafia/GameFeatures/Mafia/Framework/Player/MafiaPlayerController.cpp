// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerController.h"
#include "GameFeatures/Mafia/Framework/Character/MafiaCharacter.h"
#include "GameFeatures/Mafia/Framework/Character/MafiaSampleCharacter.h"
#include "Mafia.h"

#include "EngineUtils.h" // TActorIterator 기능을 위해 필요

void AMafiaPlayerController::CheatSetupActor()
{
	//if (GetNetMode() == ENetMode::NM_DedicatedServer)
	//{
	//	ensure(false);
	//	return;
	//}

	//UWorld* MyWorld = GetWorld();
	//if (false == IsValid(MyWorld))
	//{
	//	ensure(false);
	//	return;
	//}

	//AMafiaSampleCharacter* MyCharacter = GetPawn<AMafiaSampleCharacter>();
	//if (false == IsValid(MyCharacter))
	//{
	//	ensure(false);
	//	return;
	//}
	//FTransform SpawnedTransform = MyCharacter->GetTransform();
	//FVector SpawnedLocation = SpawnedTransform.GetLocation();
	//SpawnedLocation.X += 100.f;
	//SpawnedTransform.SetLocation(SpawnedLocation);
	//FActorSpawnParameters Params;
	//Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//if (false == IsValid(ATestCharacter))
	//{
	//	ATestCharacter = MyWorld->SpawnActor<AMafiaCharacter>(AMafiaCharacter::StaticClass(), SpawnedTransform, Params);
	//}
	//else
	//{
	//	BTestCharacter = MyWorld->SpawnActor<AMafiaCharacter>(AMafiaCharacter::StaticClass(), SpawnedTransform, Params);
	//	if (IsValid(BTestCharacter) && IsValid(ATestCharacter))
	//	{
	//		ATestCharacter->OtherCharacter = BTestCharacter;
	//	}
	//	else
	//	{
	//		ensure(false);
	//	}
	//}

	//GEngine->ForceGarbageCollection(true);
}

void AMafiaPlayerController::CheatDestoryActor()
{
	//if (GetNetMode() == ENetMode::NM_DedicatedServer)
	//{
	//	ensure(false);
	//	return;
	//}

	//UWorld* MyWorld = GetWorld();
	//if (false == IsValid(MyWorld))
	//{
	//	ensure(false);
	//	return;
	//}

	//AMafiaSampleCharacter* MyCharacter = GetPawn<AMafiaSampleCharacter>();
	//if (false == IsValid(MyCharacter))
	//{
	//	ensure(false);
	//	return;
	//}

	//for (TActorIterator<AMafiaCharacter> ActorIter(MyWorld); ActorIter; ++ActorIter)
	//{
	//	if (false == IsValid(*ActorIter))
	//	{
	//		ensure(false);
	//		continue;
	//	}

	//	if ((*ActorIter)->OtherCharacter.IsValid())
	//	{
	//		continue;
	//	}

	//	// 일단 클라에서만 동작
	//	(*ActorIter)->Destroy();
	//	MAFIA_ALOG(LogMafiaCheat, Warning, TEXT("Destroyed : %s"), *(*ActorIter)->GetName());
	//}

	//GEngine->ForceGarbageCollection(true);
	//CheatCheckActor();
}

void AMafiaPlayerController::CheatCheckActor()
{
	//AMafiaSampleCharacter* MyCharacter = GetPawn<AMafiaSampleCharacter>();
	//if (false == IsValid(MyCharacter))
	//{
	//	ensure(false);
	//	return;
	//}

	//if (IsValid(ATestCharacter))
	//{
	//	if (ATestCharacter->ArrowComp.IsValid())
	//	{
	//		MAFIA_ALOG(LogMafiaCheat, Warning, TEXT("Target->OtherCharacter is Good"));
	//	}
	//	else
	//	{
	//		MAFIA_ALOG(LogMafiaCheat, Warning, TEXT("Target->OtherCharacter is invalid"));
	//	}

	//	if (ATestCharacter->ArrowComp2.IsValid())
	//	{
	//		MAFIA_ALOG(LogMafiaCheat, Warning, TEXT("Target->OtherCharacter is Good"));
	//	}
	//	else
	//	{
	//		MAFIA_ALOG(LogMafiaCheat, Warning, TEXT("Target->OtherCharacter is Good"));
	//	}
	//}
	//else
	//{
	//	MAFIA_ALOG(LogMafiaCheat, Warning, TEXT("TargetCharacter is invalid"));
	//}
}

