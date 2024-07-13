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
	if (IsDedicatedServer())
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
	
}

bool UMafiaBaseRoleComponent::IsDedicatedServer()
{
	if (UWorld* World = GetWorld())
	{
		if (IsValid(World))
		{
			if (UMafiaGameInstance* GameInstance = World->GetGameInstance<UMafiaGameInstance>())
			{
				return GameInstance->IsDedicatedServerInstance();
			}
		}
	}
	return false;
}

void UMafiaBaseRoleComponent::ServerReqUseAbility_Implementation(AMafiaPlayerState* InOther)
{
	/** ktw : 서버에서 실행됩니다. */
	if (IsDedicatedServer() && OwningPlayerState.IsValid())
	{
		UWorld* World = GetWorld();
		if (UMafiaGameInstance* GameInstance = World->GetGameInstance<UMafiaGameInstance>())
		{
			if (GameInstance->IsDedicatedServerInstance())
			{
				if (UMafiaChairMan* ChairMan = GameInstance->GetChairMan())
				{
					ChairMan->AddAbilityEvent(OwningPlayerState.Get(), InOther);
				}
			}
		}
	}
}

void UMafiaBaseRoleComponent::ServerReqSetTeam_Implementation(EMafiaTeam InTeam)
{
	/** ktw : 서버에서 실행됩니다. */
	TeamType = InTeam;
}

void UMafiaBaseRoleComponent::ServerReqSetRole_Implementation(EMafiaRole InRole)
{
	/** ktw : 서버에서 실행됩니다. */
	RoleType = InRole;
}

void UMafiaBaseRoleComponent::ServerReqSetDead_Implementation(bool InDead)
{
	/** ktw : 서버에서 실행됩니다. */
	bDead = InDead;
}


