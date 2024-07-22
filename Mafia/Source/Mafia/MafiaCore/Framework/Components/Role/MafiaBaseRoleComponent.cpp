#include "Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "Framework/Manager/MafiaChairManManager.h"
#include "Framework/System/MafiaLogChannels.h"
#include "GameFeatures/Mafia/Framework/GameModes/MafiaGameMode.h"
#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerState.h"
#include "GameFeatures/Mafia/Framework/GameModes/MafiaGameState.h"
#include "GameFeatures/Mafia/Framework/System/MafiaGameInstance.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerController.h"
#include "GameFeatures/Mafia/Framework/Character/MafiaSampleCharacter.h"
#include "Mafia.h"
#include "Net/UnrealNetwork.h"

UMafiaBaseRoleComponent::UMafiaBaseRoleComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	CachedAffectedEventsHeap.Reserve(16);
	CachedAffectedEventsHeap.Heapify();
}

void UMafiaBaseRoleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UMafiaBaseRoleComponent, TeamType);
	DOREPLIFETIME(UMafiaBaseRoleComponent, RoleType);
	DOREPLIFETIME(UMafiaBaseRoleComponent, bDead);
	DOREPLIFETIME(UMafiaBaseRoleComponent, RoleName);
}


void UMafiaBaseRoleComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningPlayerState = Cast<AMafiaBasePlayerState>(GetOwner());
	if (OwningPlayerState.IsValid() == false)
	{
		MAFIA_ULOG(LogMafiaPlayerState, Error, TEXT("UMafiaBaseRoleComponent::BeginPlay : OwningPlayerState is Not Valid."));
	}
}

void UMafiaBaseRoleComponent::SetTeamType(EMafiaTeam InTeam)
{
	if (ENetRole::ROLE_SimulatedProxy == GetOwnerRole() || ENetRole::ROLE_AutonomousProxy)
	{
		ServerReqSetTeam(InTeam);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("클라이언트에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::SetRoleType(EMafiaRole InRole)
{
	if (ENetRole::ROLE_SimulatedProxy == GetOwnerRole() || ENetRole::ROLE_AutonomousProxy)
	{
		ServerReqSetRole(InRole);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("클라이언트에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::SetDead(bool InDead)
{
	if (ENetRole::ROLE_SimulatedProxy == GetOwnerRole() || ENetRole::ROLE_AutonomousProxy)
	{
		ServerReqSetDead(InDead);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("클라이언트에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::SetRoleName(FName InRoleName)
{
	if (ENetRole::ROLE_SimulatedProxy == GetOwnerRole() || ENetRole::ROLE_AutonomousProxy)
	{
		ServerReqSetRoleName(InRoleName);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("클라이언트에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::UseAbility(AMafiaPlayerState* InOther)
{
	if (ENetRole::ROLE_SimulatedProxy == GetOwnerRole() || ENetRole::ROLE_AutonomousProxy)
	{
		ServerReqUseAbility(InOther);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("클라이언트에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::AffectedByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientAffectedByOther(InRole, InOther);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::FlushEvents()
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientAffectedEventsFlush();
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::PostVoteEvent(AMafiaBasePlayerState* InDestination, bool InSucceed)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientPostVoteEvent(InDestination, InSucceed);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::ClientAffectedByOther_Implementation(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther)
{
	/** ktw - 클라이언트에서 실행됩니다. */
	/** Todo : ktw -  서버가 직접 이벤트를 넣어줄 지 아니면, 클라이언트가 신호를 받아서 이벤트를 넣어 두고 한 번에 Flush? */
	FAffectedEvent Event;
	Event.Other = InOther;
	CachedAffectedEventsHeap.HeapPush(Event);
}

void UMafiaBaseRoleComponent::ClientAffectedEventsFlush_Implementation()
{
	for (auto Event : CachedAffectedEventsHeap)
	{
		/** Todo : ktw - 실제 UI처리 및 동작 처리! */
	}
	CachedAffectedEventsHeap.Empty();
}

void UMafiaBaseRoleComponent::ClientPostVoteEvent_Implementation(AMafiaBasePlayerState* InDestination, bool InSucceed)
{


}

UMafiaBaseGameInstance* UMafiaBaseRoleComponent::GetServerInstance()
{
	if (UWorld* World = GetWorld())
	{
		if (IsValid(World))
		{
			if (UMafiaBaseGameInstance* GI = World->GetGameInstance<UMafiaBaseGameInstance>())
			{
				if (GI->IsDedicatedServerInstance())
				{
					return GI;
				}
				else
				{
					return nullptr;
				}
			}
		}
	}
	return nullptr;
}

void UMafiaBaseRoleComponent::ServerReqUseAbility_Implementation(AMafiaPlayerState* InOther)
{
	/** ktw - 서버에서 실행됩니다. */
	if (UMafiaBaseGameInstance* GI = GetServerInstance())
	{
		if (OwningPlayerState.IsValid())
		{
			if (UMafiaChairManManager* ChairManManager = GI->GetChairMan())
			{
				ChairManManager->AddAbilityEvent(OwningPlayerState.Get(), InOther);
			}
		}
	}
}

void UMafiaBaseRoleComponent::ServerReqSetTeam_Implementation(EMafiaTeam InTeam)
{
	/** ktw - 서버에서 실행됩니다. */
	TeamType = InTeam;
}

void UMafiaBaseRoleComponent::ServerReqSetRole_Implementation(EMafiaRole InRole)
{
	/** ktw - 서버에서 실행됩니다. */
	RoleType = InRole;
}

void UMafiaBaseRoleComponent::ServerReqSetDead_Implementation(bool InDead)
{
	/** ktw - 서버에서 실행됩니다. */
	bDead = InDead;
}

void UMafiaBaseRoleComponent::ServerReqSetRoleName_Implementation(FName InRoleName)
{
	/** ktw - 서버에서 실행됩니다. */
	RoleName = InRoleName;
}

bool operator < (const FAffectedEvent& A, const FAffectedEvent& B)
{
	if (A.Other.IsValid() && B.Other.IsValid())
	{
		return (A.Other.Get()->GetRoleType() > B.Other.Get()->GetRoleType());
	}
	else
	{
		check(false);
		return false;
	}
	return false;
}



void UMafiaBaseRoleComponent::OnRepChangeRoleType()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		if (AMafiaBasePlayerState* PS = GetOwner<AMafiaBasePlayerState>())
		{
			if (AMafiaBasePlayerController* PC = Cast<AMafiaBasePlayerController>(PS->GetPlayerController()))
			{
				if (AMafiaSampleCharacter* Character = Cast<AMafiaSampleCharacter>(PC->GetPawn()))
				{
					switch (RoleType)
					{
					case EMafiaRole::Madam:
						break;
					case EMafiaRole::Police:
						Character->ChangeColor(FLinearColor(0, 0, 1, 0), 0);
						break;
					case EMafiaRole::Killer:
						break;
					case EMafiaRole::Mafia:
						Character->ChangeColor(FLinearColor(1, 0, 0, 0), 0);
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
						Character->ChangeColor(FLinearColor(1, 1, 0, 0), 0);
						break;
					case EMafiaRole::Citizen:
						break;
					default:
						break;
					}
				}
			}
		}
	}
}
