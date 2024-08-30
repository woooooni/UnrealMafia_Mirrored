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
#include "Framework/System/MafiaGameEvents.h"

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
	
	DOREPLIFETIME(UMafiaBaseRoleComponent, bDead);
	DOREPLIFETIME(UMafiaBaseRoleComponent, RoleName);
	DOREPLIFETIME(UMafiaBaseRoleComponent, TeamType);
	DOREPLIFETIME(UMafiaBaseRoleComponent, RoleType);
	DOREPLIFETIME(UMafiaBaseRoleComponent, OwningPlayerState);
	DOREPLIFETIME(UMafiaBaseRoleComponent, CachedAffectedEventsHeap);
}

void UMafiaBaseRoleComponent::OnUnregister()
{
	Super::OnUnregister();
	if (GetOwnerRole() != ROLE_Authority)
	{
		UnBindDelegates();
	}
}


void UMafiaBaseRoleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPlayerState = Cast<AMafiaBasePlayerState>(GetOwner());
	if (OwningPlayerState.IsValid() == false)
	{
		MAFIA_ULOG(LogMafiaPlayerState, Error, TEXT("UMafiaBaseRoleComponent::BeginPlay : OwningPlayerState is Invalid."));
	}

	if (GetOwnerRole() != ROLE_Authority)
	{
		BindDelegates();
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

void UMafiaBaseRoleComponent::SetRoleName(FName InRoleName)
{
	ServerReqSetRoleName(InRoleName);
}

void UMafiaBaseRoleComponent::SetAffectedEvents(const TArray<FAffectedEvent>& InEvents)
{
	ServerReqSetAffectedEvents(InEvents);
}





#pragma region Ability

void UMafiaBaseRoleComponent::BindDelegates()
{
	OnChangedMafiaFlowStateHandle = BindGameEvent(OnChangedMafiaFlowState, &UMafiaBaseRoleComponent::OnChangedMafiaFlowState);
}

void UMafiaBaseRoleComponent::UnBindDelegates()
{
	UnbindGameEvent(OnChangedMafiaFlowState, OnChangedMafiaFlowStateHandle);
}

void UMafiaBaseRoleComponent::UseAbility(AMafiaBasePlayerState* InOther)
{
	ServerReqUseAbility(InOther, EMafiaAbilityEventType::DefferedEvent);
}

void UMafiaBaseRoleComponent::SendOfferMafiaTeam(AMafiaBasePlayerState* InOther)
{
	if (RoleType == EMafiaRole::Mafia)
	{
		ServerReqSendOfferMafiaTeam(InOther);
	}
}

void UMafiaBaseRoleComponent::AffectedAbilityByOther(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther)
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


void UMafiaBaseRoleComponent::NotifyResultAbility(UMafiaBaseRoleComponent* InOther)
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



void UMafiaBaseRoleComponent::RecieveInstantEvent(UMafiaBaseRoleComponent* InOther)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientRecieveInstantEvent(InOther);
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


void UMafiaBaseRoleComponent::ResponseVoteEvent(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientResponseVoteEvent(InCandidate, InFlag);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::ReceiveVoteResult(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientReceiveVoteResult(InDeathRow, InFlag);
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
void UMafiaBaseRoleComponent::ServerReqUseAbility_Implementation(AMafiaBasePlayerState* InOther, EMafiaAbilityEventType InEventType)
{
	/** ktw : 서버에서 실행됩니다. */
	if (UMafiaChairManManager* ChairManManager = GetChairMan())
	{
		if (OwningPlayerState.IsValid())
		{
			EMafiaUseAbilityFlag Flag = ChairManManager->AddAbilityEvent(OwningPlayerState.Get(), InOther, InEventType);
			ClientResponseUseAbility(InOther->GetRoleComponent(), Flag, InEventType);
		}
	}
}


void UMafiaBaseRoleComponent::ServerReqSendOfferMafiaTeam_Implementation(AMafiaBasePlayerState* InOther)
{
	if (RoleType == EMafiaRole::Mafia)
	{
		if (UMafiaChairManManager* ChairManManager = GetChairMan())
		{
			if (OwningPlayerState.IsValid())
			{
				ChairManManager->AddAbilityEvent(OwningPlayerState.Get(), InOther, EMafiaAbilityEventType::InstantEvent);
			}
		}
	}
	
}



void UMafiaBaseRoleComponent::ClientAffectedAbilityByOther_Implementation(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther)
{
	/** ktw : 클라이언트에서 실행됩니다. */
	if (IsValid(InOther))
	{
		FAffectedEvent Event;
		Event.Other = InOther;
		CachedAffectedEventsHeap.HeapPush(Event);
	}
	else
	{
		ensureMsgf(false, TEXT("InOther is Invalid."));
	}
}

void UMafiaBaseRoleComponent::ClientResponseUseAbility_Implementation(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag, EMafiaAbilityEventType InEventType)
{
	/** #Todo - ktw : 파생 클래스에서 override. Flag에 따른 동작 처리. */

}

void UMafiaBaseRoleComponent::ClientNotifyResultAbility_Implementation(UMafiaBaseRoleComponent* InOther)
{
	/** #Todo - ktw : 파생 클래스에서 override. 자신의 능력 사용에 관한 결과를 계산합니다. (경찰, 마피아, 대부, .. 등등) */
}

void UMafiaBaseRoleComponent::ClientAffectedEventsFlush_Implementation()
{
	/** ktw : 파생 클래스에서 override. */
}

void UMafiaBaseRoleComponent::ClientRecieveInstantEvent_Implementation(UMafiaBaseRoleComponent* InOther)
{
	/** ktw : 파생 클래스에서 override. */
}




#pragma endregion RPC Ability

#pragma region RPC Vote
void UMafiaBaseRoleComponent::ServerReqVote_Implementation(AMafiaBasePlayerState* InOther)
{
	/** ktw : 서버에서 실행됩니다. */
		/** ktw : 서버에서 실행됩니다. */
	if (UMafiaChairManManager* ChairManManager = GetChairMan())
	{
		if (OwningPlayerState.IsValid())
		{
			ChairManManager->AddVoteEvent(OwningPlayerState.Get(), InOther);
		}
	}
}

void UMafiaBaseRoleComponent::ClientStartVoteEvent_Implementation()
{
	/** ktw : 클라이언트에서 실행됩니다. */

}

void UMafiaBaseRoleComponent::ClientResponseVoteEvent_Implementation(AMafiaBasePlayerState* InCandidate, EMafiaVoteFlag InFlag)
{
	/** 
		ktw : 클라이언트에서 실행됩니다. 
				InCandidate는 nullptr일 수 있습니다.
	*/
	
	// 투표 결과 응답.
	if (OwningPlayerState.IsValid())
	{
		/** Todo - ktw : 투표 Flag당 실행할 기능 추가. */
		if (EMafiaVoteFlag::ImpossibleVote == InFlag)
		{
			// EMafiaGameFlow가 Vote가 아니거나 투표를 할 수 없는 상태.
		}
		else if (EMafiaVoteFlag::AlreadyVoted == InFlag)
		{
			// 중복 투표
		}
		else if (EMafiaVoteFlag::Succeed == InFlag)
		{
			// 투표 완료.
		}
		else
		{

		}
	}
}

void UMafiaBaseRoleComponent::ClientReceiveVoteResult_Implementation(UMafiaBaseRoleComponent* InDeathRow, EMafiaVoteResultFlag InFlag)
{
	/** #Todo - ktw : 통지된 사형수가 있는지 체크해서 별도의 동작을 수행. */
	if (InFlag == EMafiaVoteResultFlag::NoDeathPlayer)
	{

	}
	else if (InFlag == EMafiaVoteResultFlag::SomeoneDying)
	{

	}
	
}

void UMafiaBaseRoleComponent::ClientFinishVoteEvent_Implementation()
{
	/** ktw : 클라이언트에서 실행됩니다. */
}

#pragma endregion RPC Vote







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

void UMafiaBaseRoleComponent::ServerReqSetRoleName_Implementation(FName InRoleName)
{
	/** ktw : 서버에서 실행됩니다. */
	RoleName = InRoleName;
}

void UMafiaBaseRoleComponent::ServerReqSetAffectedEvents_Implementation(const TArray<FAffectedEvent>& InEvents)
{
	/** ktw : 서버에서 실행됩니다. */
	CachedAffectedEventsHeap = InEvents;
}


void UMafiaBaseRoleComponent::OnChangedMafiaFlowState(const EMafiaFlowState& InMafiaFlowState)
{
	/** #Todo - ktw : Flow에 따른 연출 및 UI 출력. */
	if (InMafiaFlowState == EMafiaFlowState::None)
	{
		
	}
	else if(InMafiaFlowState == EMafiaFlowState::BeforeDay)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::Day)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::AfterDay)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::EndDay)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::BeforeVote)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::Vote)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::AfterVote)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::EndVote)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::BeforeNight)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::Night)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::AfterNight)
	{

	}
	else if(InMafiaFlowState == EMafiaFlowState::EndNight)
	{

	}
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

UMafiaChairManManager* UMafiaBaseRoleComponent::GetChairMan()
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


