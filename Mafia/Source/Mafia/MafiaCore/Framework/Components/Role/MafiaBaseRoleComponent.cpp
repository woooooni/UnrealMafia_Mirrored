#include "Framework/Components/Role/MafiaBaseRoleComponent.h"
#include "Framework/Manager/MafiaChairmanManager.h"
#include "Framework/System/MafiaLogChannels.h"
#include "GameFeatures/Mafia/Framework/GameModes/MafiaGameMode.h"
#include "GameFeatures/Mafia/Framework/Player/MafiaPlayerState.h"
#include "GameFeatures/Mafia/Framework/GameModes/MafiaGameState.h"
#include "GameFeatures/Mafia/Framework/System/MafiaGameInstance.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerController.h"
#include "GameFeatures/Mafia/Framework/Character/MafiaSampleCharacter.h"
#include "Mafia.h"
#include "Net/UnrealNetwork.h"
#include "Framework/System/MafiaGameEvents.h"

UMafiaBaseRoleComponent::UMafiaBaseRoleComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	CachedAffectedEventsHeap.Reserve(16);
	CachedBroadCastEventsHeap.Reserve(16);
	HandleAbilityEventTime = 2.f;
	HandleBroadCastEventTime = 2.f;
}

void UMafiaBaseRoleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UMafiaBaseRoleComponent, bDead);
	DOREPLIFETIME(UMafiaBaseRoleComponent, RoleName);
	DOREPLIFETIME(UMafiaBaseRoleComponent, TeamType);
	DOREPLIFETIME(UMafiaBaseRoleComponent, RoleType);
	DOREPLIFETIME(UMafiaBaseRoleComponent, OwningPlayerState);
}




void UMafiaBaseRoleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPlayerState = Cast<AMafiaBasePlayerState>(GetOwner());
	if (OwningPlayerState.IsValid() == false)
	{
		MAFIA_ULOG(LogMafiaPlayerState, Error, TEXT("UMafiaBaseRoleComponent::BeginPlay : OwningPlayerState is Invalid."));
	}

	BindDelegates();
}

void UMafiaBaseRoleComponent::OnUnregister()
{
	Super::OnUnregister();
	UnBindDelegates();
}

void UMafiaBaseRoleComponent::SetTeamType(EMafiaTeam InTeam)
{
	ServerReqSetTeamType(InTeam);
}

void UMafiaBaseRoleComponent::SetRoleType(EMafiaRole InRole)
{
	ServerReqSetRoleType(InRole);
}

void UMafiaBaseRoleComponent::SetDead(bool InDead)
{
	ServerReqSetDead(InDead);
}

void UMafiaBaseRoleComponent::SetRoleName(FName InRoleName)
{
	ServerReqSetRoleName(InRoleName);
}

void UMafiaBaseRoleComponent::BindDelegates()
{
	OnChangedMafiaFlowStateHandle = BindGameEvent(OnChangedMafiaFlowState, &UMafiaBaseRoleComponent::OnChangedMafiaFlowState);
}

void UMafiaBaseRoleComponent::UnBindDelegates()
{
	UnbindGameEvent(OnChangedMafiaFlowState, OnChangedMafiaFlowStateHandle);
}

void UMafiaBaseRoleComponent::ResetForNextRound()
{
	CachedProcessedAbilityEventsSet.Empty();
	CachedProcessedBroadCastEventsSet.Empty();
	CachedAffectedEventsHeap.Empty();
	CachedBroadCastEventsHeap.Empty();
}


#pragma region Ability

void UMafiaBaseRoleComponent::UseAbility(AMafiaBasePlayerState* InOther)
{
	ServerReqUseAbility(InOther, AbilityEventType);
}

void UMafiaBaseRoleComponent::SendBroadCastEvent(const EMafiaBroadCastEvent& InEvent)
{
	ServerReqSendBroadCastEvent(InEvent);
}

void UMafiaBaseRoleComponent::ReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent)
{
	if (ROLE_Authority == GetOwnerRole())
	{
		ClientReceiveBroadCastEvent(InSender, InEvent);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::AffectedAbilityByOther(EMafiaRole InRole, AMafiaBasePlayerState* InOther)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientAffectedAbilityByOther(InRole, InOther);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}


void UMafiaBaseRoleComponent::NotifyResultAbility(AMafiaBasePlayerState* InOther)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientNotifyResultAbility(InOther);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}

#pragma endregion Ability


#pragma region Vote
void UMafiaBaseRoleComponent::Vote(AMafiaBasePlayerState* InOther)
{
	ServerReqVote(InOther);
}



void UMafiaBaseRoleComponent::ReceiveInstantEvent(AMafiaBasePlayerState* InOther)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientReceiveInstantEvent(InOther);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}



void UMafiaBaseRoleComponent::StartVoteEvent()
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientStartVoteEvent();
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::ReceiveVoteResult(AMafiaBasePlayerState* InDeadMan, EMafiaVoteResultFlag InFlag)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientReceiveVoteResult(InDeadMan, InFlag);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}


void UMafiaBaseRoleComponent::FinishVoteEvent()
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientFinishVoteEvent();
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}



#pragma endregion Vote


#pragma region RPC Ability

void UMafiaBaseRoleComponent::ServerReqSendBroadCastEvent_Implementation(const EMafiaBroadCastEvent InEvent)
{
	/** ktw : 서버에서 실행됩니다. */
	if (UMafiaChairmanManager* ChairManManager = GetChairMan())
	{
		if (OwningPlayerState.IsValid())
		{
			EMafiaBroadCastEventFlag Flag = ChairManManager->BroadCastEvent(OwningPlayerState.Get(), InEvent);
		}
	}
}

void UMafiaBaseRoleComponent::ServerReqUseAbility_Implementation(AMafiaBasePlayerState* InOther, EMafiaAbilityEventType InEventType)
{
	/** ktw : 서버에서 실행됩니다. */
	if (UMafiaChairmanManager* ChairManManager = GetChairMan())
	{
		if (OwningPlayerState.IsValid())
		{
			EMafiaUseAbilityFlag Flag = ChairManManager->AddAbilityEvent(OwningPlayerState.Get(), InOther, InEventType);
			ClientResponseUseAbility(InOther, Flag, InEventType);
		}
	}
}


void UMafiaBaseRoleComponent::ClientAffectedAbilityByOther_Implementation(EMafiaRole InRole, AMafiaBasePlayerState* InOther)
{
	HandleReceiveAffectedAbilityEvent(InRole, InOther);
}

void UMafiaBaseRoleComponent::ClientResponseUseAbility_Implementation(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	HandleResponseUseAbilityEvent(InOther, InFlag, InEventType);
}

void UMafiaBaseRoleComponent::ClientNotifyResultAbility_Implementation(AMafiaBasePlayerState* InOther)
{
	HandleNotifyResultAbilityEvent(InOther);
}

void UMafiaBaseRoleComponent::ClientAbilityEventsFlush_Implementation()
{
	HandleAffectedAbilityEvents();
}

void UMafiaBaseRoleComponent::ClientReceiveInstantEvent_Implementation(AMafiaBasePlayerState* InOther)
{
	HandleReceiveInstantEvent(InOther);
}

void UMafiaBaseRoleComponent::ClientReceiveBroadCastEvent_Implementation(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent InEvent)
{
	HandleReceiveBroadCastEvent(InSender, InEvent);
}




#pragma endregion RPC Ability

#pragma region RPC Vote
void UMafiaBaseRoleComponent::ServerReqVote_Implementation(AMafiaBasePlayerState* InOther)
{
	/** ktw : 서버에서 실행됩니다. */
		/** ktw : 서버에서 실행됩니다. */
	if (UMafiaChairmanManager* ChairManManager = GetChairMan())
	{
		if (OwningPlayerState.IsValid())
		{
			EMafiaVoteFlag Flag = ChairManManager->AddVoteEvent(OwningPlayerState.Get(), InOther);
			ClientResponseVoteEvent(InOther, Flag);
		}
	}
}

void UMafiaBaseRoleComponent::ClientStartVoteEvent_Implementation()
{
	/** ktw : 클라이언트에서 실행됩니다. */
	HandleStartVoteEvent();
}

void UMafiaBaseRoleComponent::ClientResponseVoteEvent_Implementation(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
	/** ktw : 클라이언트에서 실행됩니다. */
	HandleResponseVoteEvent(InCandidate, InFlag);
}

void UMafiaBaseRoleComponent::ClientReceiveVoteResult_Implementation(AMafiaBasePlayerState* InDeadMan, EMafiaVoteResultFlag InFlag)
{
	HandleReceiveVoteResultEvent(InDeadMan, InFlag);
}

void UMafiaBaseRoleComponent::ClientFinishVoteEvent_Implementation()
{
	HandleFinishVoteEvent();
}

#pragma endregion RPC Vote







void UMafiaBaseRoleComponent::ServerReqSetTeamType_Implementation(EMafiaTeam InTeam)
{
	/** ktw : 서버에서 실행됩니다. */
	TeamType = InTeam;
}

void UMafiaBaseRoleComponent::ServerReqSetRoleType_Implementation(EMafiaRole InRole)
{
	/** ktw : 서버에서 실행됩니다. */
	RoleType = InRole;
}

void UMafiaBaseRoleComponent::ServerReqSetDead_Implementation(bool InDead)
{
	/** ktw : 서버에서 실행됩니다. */
	bDead = InDead;
}

void UMafiaBaseRoleComponent::ServerReqSetRoleName_Implementation(FName InRoleName)
{
	/** ktw : 서버에서 실행됩니다. */
	RoleName = InRoleName;
}


void UMafiaBaseRoleComponent::OnChangedMafiaFlowState(const EMafiaFlowState& InMafiaFlowState)
{
	/** #Todo - ktw : Flow에 따른 연출 및 UI 출력. */
	if (InMafiaFlowState == EMafiaFlowState::None || InMafiaFlowState == EMafiaFlowState::EndVote || InMafiaFlowState == EMafiaFlowState::EndPunishment)
	{
		ResetForNextRound();
	}

	else if (InMafiaFlowState == EMafiaFlowState::EndNight)
	{
		HandleAffectedAbilityEvents();
	}

	else if (InMafiaFlowState == EMafiaFlowState::BeforeDay)
	{
		HandleBroadCastEvents();
	}
}

void UMafiaBaseRoleComponent::HandleAffectedAbilityEvents()
{
	for (auto& Event : CachedAffectedEventsHeap)
	{
		CachedProcessedAbilityEventsSet.Emplace(Event.AbilityPlayerRole);
	}
	
	if (!CachedAffectedEventsHeap.IsEmpty())
	{
		FAffectedEvent OutAffectedEvent;
		CachedAffectedEventsHeap.HeapPop(OutAffectedEvent);

		HandleAffectedAbilityEvent(OutAffectedEvent);

		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			World->GetTimerManager().SetTimer(AffectedAbilityTimerHandle, this, &UMafiaBaseRoleComponent::HandleAffectedAbilityEvents, HandleAbilityEventTime);
		}
	}
}

void UMafiaBaseRoleComponent::HandleAffectedAbilityEvent(const FAffectedEvent& InEvent)
{
	
}

void UMafiaBaseRoleComponent::HandleBroadCastEvents()
{
	for (auto& Event : CachedBroadCastEventsHeap)
	{
		CachedProcessedBroadCastEventsSet.Emplace(Event.EventType);
	}

	if (!CachedBroadCastEventsHeap.IsEmpty())
	{
		FBroadCastEvent OutBroadCastEvent;
		CachedBroadCastEventsHeap.HeapPop(OutBroadCastEvent);

		HandleBroadCastEvent(OutBroadCastEvent);

		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			World->GetTimerManager().SetTimer(BroadCastEventTimerHandle, this, &UMafiaBaseRoleComponent::HandleBroadCastEvents, HandleBroadCastEventTime);
		}
	}
}

void UMafiaBaseRoleComponent::HandleBroadCastEvent(const FBroadCastEvent& InEvent)
{

}

void UMafiaBaseRoleComponent::HandleReceiveAffectedAbilityEvent(EMafiaRole InRole, AMafiaBasePlayerState* InOther)
{
	if (IsValid(InOther))
	{
		FAffectedEvent Event;
		Event.AbilityPlayer = InOther;
		Event.AbilityPlayerRole = InRole;
		CachedAffectedEventsHeap.HeapPush(Event);
	}
}

void UMafiaBaseRoleComponent::HandleReceiveBroadCastEvent(AMafiaBasePlayerState* InSender, const EMafiaBroadCastEvent& InEvent)
{
	if (IsValid(InSender))
	{
		FBroadCastEvent Event;
		Event.EventSender = InSender;
		Event.EventType = InEvent;

		CachedBroadCastEventsHeap.HeapPush(Event);\
	}
}

void UMafiaBaseRoleComponent::HandleResponseUseAbilityEvent(AMafiaBasePlayerState* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	SendGameEvent_ThreeParams(OnResponseUseAbility, InOther, InFlag, InEventType);
}


UMafiaChairmanManager* UMafiaBaseRoleComponent::GetChairMan()
{
	if (UWorld* World = GetWorld())
	{
		if (IsValid(World))
		{
			if (UMafiaBaseGameInstance* GI = World->GetGameInstance<UMafiaBaseGameInstance>())
			{
				if (GI->IsDedicatedServerInstance())
				{
					return GI->GetChairMan();
				}
			}
		}
	}
	return nullptr;
}
