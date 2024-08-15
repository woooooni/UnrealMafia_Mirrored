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
	UnbindGameEvent(OnChangedMafiaFlowState, OnChangedMafiaFlowStateHandle);
}


void UMafiaBaseRoleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPlayerState = Cast<AMafiaBasePlayerState>(GetOwner());
	if (OwningPlayerState.IsValid() == false)
	{
		MAFIA_ULOG(LogMafiaPlayerState, Error, TEXT("UMafiaBaseRoleComponent::BeginPlay : OwningPlayerState is Invalid."));
	}

	OnChangedMafiaFlowStateHandle = BindGameEvent(OnChangedMafiaFlowState, &UMafiaBaseRoleComponent::OnChangedMafiaFlowState);
	
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

void UMafiaBaseRoleComponent::UseAbility(AMafiaBasePlayerState* InOther)
{
	ServerReqUseAbility(InOther);
}

void UMafiaBaseRoleComponent::SendOfferMafiaTeam(AMafiaBasePlayerState* InOther)
{
	ServerReqSendOfferMafiaTeam(InOther);
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


void UMafiaBaseRoleComponent::ResponseUseAbility(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientResponseUseAbility(InOther, InFlag);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}



void UMafiaBaseRoleComponent::AffectedEventsFlush()
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

void UMafiaBaseRoleComponent::BusRide(const TArray<FAffectedEvent>& InEventArray)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientBusRide(InEventArray);
	}
	else
	{
		MAFIA_ULOG(LogMafiaCharacter, Warning, TEXT("서버에서 호출해야합니다."));
	}
}

void UMafiaBaseRoleComponent::RecieveOfferMafiaTeam(UMafiaBaseRoleComponent* InMafiaComponent)
{
	if (ENetRole::ROLE_Authority == GetOwnerRole())
	{
		ClientRecieveOfferMafiaTeam(InMafiaComponent);
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
void UMafiaBaseRoleComponent::ServerReqUseAbility_Implementation(AMafiaBasePlayerState* InOther)
{
	/** ktw : 서버에서 실행됩니다. */
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

void UMafiaBaseRoleComponent::ServerReqSendOfferMafiaTeam_Implementation(AMafiaBasePlayerState* InOther)
{
	if (RoleType == EMafiaRole::Mafia)
	{
		if (UMafiaBaseGameInstance* GI = GetServerInstance())
		{
			/*if (OwningPlayerState.IsValid())
			{
				if (UMafiaChairManManager* ChairManManager = GI->GetChairMan())
				{
					ChairManManager->AddAbilityEvent(OwningPlayerState.Get(), InOther);
				}
			}*/
		}
	}
	
}



void UMafiaBaseRoleComponent::ClientAffectedAbilityByOther_Implementation(EMafiaRole InRole, UMafiaBaseRoleComponent* InOther)
{
	/** ktw : 클라이언트에서 실행됩니다. */
	/** Todo - ktw :  서버가 직접 이벤트를 넣어줄 지 아니면, 클라이언트가 신호를 받아서 이벤트를 넣어 두고 한 번에 Flush? */
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

void UMafiaBaseRoleComponent::ClientResponseUseAbility_Implementation(UMafiaBaseRoleComponent* InOther, EMafiaUseAbilityFlag InFlag)
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

void UMafiaBaseRoleComponent::ClientRecieveOfferMafiaTeam_Implementation(UMafiaBaseRoleComponent* InMafiaComponent)
{
	/** #Todo - ktw : 파생 클래스에서 override. 마피아 영입 제안 */
}

void UMafiaBaseRoleComponent::ClientBusRide_Implementation(const TArray<FAffectedEvent>& InNewEventArray)
{
	/** #Todo - ktw : 버스기사에 영향을 받은 후 사운드 출력, UI 출력 등. Event는 아직 Relicated 되기 이전. */
	ServerReqSetAffectedEvents(InNewEventArray);
}



#pragma endregion RPC Ability

#pragma region RPC Vote
void UMafiaBaseRoleComponent::ServerReqVote_Implementation(AMafiaBasePlayerState* InOther)
{
	/** ktw : 서버에서 실행됩니다. */
		/** ktw : 서버에서 실행됩니다. */
	if (UMafiaBaseGameInstance* GI = GetServerInstance())
	{
		if (OwningPlayerState.IsValid())
		{
			if (UMafiaChairManManager* ChairManManager = GI->GetChairMan())
			{
				ChairManManager->AddVoteEvent(OwningPlayerState.Get(), InOther);
			}
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
		uint8 Test = 0;
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




//void UMafiaBaseRoleComponent::OnRepChangeRoleType()
//{
//	/*if (GetOwnerRole() == ROLE_AutonomousProxy)
//	{
//		if (AMafiaBasePlayerState* PS = GetOwner<AMafiaBasePlayerState>())
//		{
//
//			switch (RoleType)
//			{
//			case EMafiaRole::Madam:
//				break;
//			case EMafiaRole::Police:
//				Character->ChangeColor(FLinearColor(0, 0, 1));
//				break;
//			case EMafiaRole::Killer:
//				break;
//			case EMafiaRole::Mafia:
//				Character->ChangeColor(FLinearColor(1, 0, 0));
//				break;
//			case EMafiaRole::Vigilante:
//				break;
//			case EMafiaRole::BusDriver:
//				break;
//			case EMafiaRole::Detective:
//				break;
//			case EMafiaRole::Spirit:
//				break;
//			case EMafiaRole::Thief:
//				break;
//			case EMafiaRole::Soldier:
//				break;
//			case EMafiaRole::Doctor:
//				Character->ChangeColor(FLinearColor(1, 1, 0), 0);
//				break;
//			case EMafiaRole::Citizen:
//				break;
//			default:
//				break;
//			}
//		}
//	}*/
//}


