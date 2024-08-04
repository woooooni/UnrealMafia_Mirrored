// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameMode.h"
#include "MafiaCore/Framework/GameModes/MafiaBaseGameState.h"
#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "GameFeatures/Mafia/Framework/Components/Role/MafiaCitizenRoleComponent.h"
#include "GameFeatures/Mafia/Framework/Components/Role/MafiaPoliceRoleComponent.h"
#include "GameFeatures/Mafia/Framework/Components/Role/MafiaDoctorRoleComponent.h"
#include "GameFeatures/Mafia/Framework/Components/Role/MafiaGodFatherRoleComponent.h"
#include "GameFeatures/Mafia/Framework/Character/MafiaSampleCharacter.h"

#include "Net/UnrealNetwork.h"
#include "Mafia.h"
#include "Mafia/Framework/Player/MafiaPlayerController.h"


AMafiaBasePlayerState::AMafiaBasePlayerState(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

}

void AMafiaBasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMafiaBasePlayerState, bReadyForGame);
	DOREPLIFETIME(AMafiaBasePlayerState, RoleComponent);
	DOREPLIFETIME(AMafiaBasePlayerState, PlayerColor);
}

void AMafiaBasePlayerState::BeginPlay()
{
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("Begin"));
	Super::BeginPlay();
	if (HasAuthority())
	{
		if (UWorld* World = GetWorld())
		{
			if (APlayerController* PC = GetPlayerController())
			{
				AMafiaSampleCharacter* DefaultCharacter = World->SpawnActor<AMafiaSampleCharacter>(AMafiaSampleCharacter::StaticClass(), FVector(0, 0, 60), FRotator(0, 0, 0));
				PC->SetPawn(DefaultCharacter);
			}
		}
	}
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("End. GetOwner:%s"), GetOwner() ? *GetOwner()->GetName() : TEXT("Other Client"));
}

void AMafiaBasePlayerState::PostInitializeComponents()
{
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("Begin"));
	Super::PostInitializeComponents();
	MAFIA_ALOG(LogMafiaPlayerState, Log, TEXT("End"));
}

UMafiaBaseRoleComponent* AMafiaBasePlayerState::AssignAbility(EMafiaRole InRole)
{
	if (HasAuthority())
	{
		RoleComponent = CreateRoleComponent(InRole);
		if (IsValid(RoleComponent))
		{
			PostInitializeRoleComponent();
		}
		return RoleComponent;
	}
	
	return nullptr;
}

void AMafiaBasePlayerState::ChangePlayerColor(FLinearColor InColor)
{
	ServerChangePlayerColor(InColor);
}

void AMafiaBasePlayerState::ServerChangePlayerColor_Implementation(FLinearColor InColor)
{
	/** ktw : 서버에서 실행됩니다. */
	PlayerColor = InColor;

	if (AMafiaBasePlayerController* PC = Cast<AMafiaBasePlayerController>(GetPlayerController()))
	{
		if (AMafiaSampleCharacter* Character = Cast<AMafiaSampleCharacter>(PC->GetPawn()))
		{
			Character->ChangeColor(PlayerColor);
		}
	}
}


UMafiaBaseRoleComponent* AMafiaBasePlayerState::CreateRoleComponent(EMafiaRole InRole)
{
	if (IsValid(RoleComponent))
	{
		RoleComponent->DestroyComponent();
		RoleComponent = nullptr;
	}

	UMafiaBaseRoleComponent* NewRoleComponent = nullptr;
	switch (InRole)
	{
	case EMafiaRole::Citizen:
		NewRoleComponent = NewObject<UMafiaCitizenRoleComponent>(this);
		break;
	case EMafiaRole::Madam:
		break;
	case EMafiaRole::Police:
		NewRoleComponent = NewObject<UMafiaPoliceRoleComponent>(this);
		break;
	case EMafiaRole::Killer:
		break;
	case EMafiaRole::Mafia:
		NewRoleComponent = NewObject<UMafiaGodFatherRoleComponent>(this);
		break;
	case EMafiaRole::Vigilante:
		break;
	case EMafiaRole::BusDriver:
		break;
	case EMafiaRole::Detective:
		break;
	case EMafiaRole::Spirit:
		break;
	case EMafiaRole::Thief:
		break;
	case EMafiaRole::Soldier:
		break;
	case EMafiaRole::Doctor:
		NewRoleComponent = NewObject<UMafiaDoctorRoleComponent>(this);
		break;
	default:
		return nullptr;
	}

	if (IsValid(NewRoleComponent))
	{
		NewRoleComponent->SetIsReplicated(true);
		NewRoleComponent->RegisterComponent();
	}

	return NewRoleComponent;
}

void AMafiaBasePlayerState::PostInitializeRoleComponent_Implementation()
{
	/**	
		### 클라이언트에서는 RoleComponent가 아직 리플리케이션 되지 않아 Valid 체크가 실패할 수 있습니다. ###
		#Todo : ktw - Role Component를 생성하고 수행할 동작을 구현합니다.
		
		1. 클라이언트에서 실행됩니다. 
		2. Server에서 Role Component가 생성된 후, 호출됩니다.
		3. 좋은 함수이름있으면 변경해도 됩니다.
	*/
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