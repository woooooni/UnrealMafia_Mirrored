#include "MafiaCore/Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "GameFeatures/Mafia/Framework/GameModes/MafiaGameMode.h"
#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerState.h"
#include "MafiaCore/Framework/System/MafiaLogChannels.h"
#include "GameFeatures/Mafia/Framework/GameModes/MafiaGameState.h"
#include "GameFeatures/Mafia/Framework/System/MafiaGameInstance.h"
#include "Mafia.h"
#include "Net/UnrealNetwork.h"

UMafiaBaseRoleComponent::UMafiaBaseRoleComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	CachedAffectedEventsHeap.Reserve(16);
	CachedAffectedEventsHeap.Heapify();
}

void UMafiaBaseRoleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UMafiaBaseRoleComponent, TeamType);
	DOREPLIFETIME(UMafiaBaseRoleComponent, RoleType);
	DOREPLIFETIME(UMafiaBaseRoleComponent, bDead);

}


void UMafiaBaseRoleComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}

void UMafiaBaseRoleComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if (GetServerInstance())
	{
		OwningPlayerState = GetOwner<AMafiaPlayerState>();
		if (OwningPlayerState.IsValid() == false)
		{
			MAFIA_ULOG(LogMafiaPlayerState, Warning, TEXT("UMafiaBaseRoleComponent::BeginPlay : No Has PlayerState"));
		}
	}
}

void UMafiaBaseRoleComponent::SetTeamType(EMafiaTeam InTeam)
{
	ServerReqSetTeam(InTeam);
}

void UMafiaBaseRoleComponent::SetRoleType(EMafiaRole InRole)
{
	ServerReqSetRole(InRole);
}

void UMafiaBaseRoleComponent::SetDead(bool InDead)
{
	ServerReqSetDead(InDead);
}

void UMafiaBaseRoleComponent::UseAbility(AMafiaPlayerState* InOther)
{
	ServerReqUseAbility(InOther);
}

void UMafiaBaseRoleComponent::AffectedByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther)
{
	ClientAffectedByOther(InRole, InOther);
}

void UMafiaBaseRoleComponent::ClientAffectedByOther_Implementation(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther)
{
	/** ktw - 클라이언트에서 실행됩니다. */
	/** Todo : ktw -  서버가 직접 이벤트를 넣어줄 지 아니면, 클라이언트가 신호를 받아서 이벤트를 넣어 두고 한 번에 Flush? */
	FAffectedEvent Event;
	Event.Other = InOther;
	CachedAffectedEventsHeap.HeapPush(Event);
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
			if (UMafiaChairMan* ChairMan = GI->GetChairMan())
			{
				ChairMan->AddAbilityEvent(OwningPlayerState.Get(), InOther);
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


