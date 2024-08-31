// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeatures/Mafia/Framework/Character/MafiaSampleCharacter.h"
#include "Mafia/Framework/UI/InGame/MafiaPlayerNameActionGroupWidget.h"
#include "MafiaCore/Framework/Player/MafiaBasePlayerState.h"
#include "Engine/LocalPlayer.h"


#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"


#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "GameFeatures/Mafia/Framework/UI/InGame/MafiaPlayerNameActionGroupWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMafiaSampleCharacter

AMafiaSampleCharacter::AMafiaSampleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = false;
	CameraBoom->SetRelativeLocationAndRotation(FVector(0, 0, 44), FRotator(-30, 0, 0));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, 0, -90));

	PlayerNameComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerNameWidget"));
	PlayerNameComponent->SetupAttachment(RootComponent);
	PlayerNameComponent->SetRelativeLocation(FVector(0.f, 0.f, 125.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerNameWidgetRef = TEXT("/Game/UI/ActionGruopWidget/InGame/WBP_MafiaPlayerNameActionGroupWidget.WBP_MafiaPlayerNameActionGroupWidget_C");
	if (PlayerNameWidgetRef.Succeeded())
	{
		PlayerNameComponent->SetWidgetClass(PlayerNameWidgetRef.Class);
		PlayerNameComponent->SetWidgetSpace(EWidgetSpace::Screen);
		PlayerNameComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayerNameComponent->SetVisibility(true);
	}
}

void AMafiaSampleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMafiaSampleCharacter, CharacterColor);
	DOREPLIFETIME(AMafiaSampleCharacter, PlayerName);
}

void AMafiaSampleCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// SetPlayerColor
	if (AMafiaBasePlayerState* PS = GetPlayerState<AMafiaBasePlayerState>())
	{
		PS->ChangePlayerColor(PS->GetPlayerColor());
	}
}

void AMafiaSampleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 카메라 위치와의 거리 계산
	
	FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	float Distance = FVector::Dist(CameraLocation, GetActorLocation());

	// 거리에 따른 크기 계산 (예시: 가까울수록 크기 작음.)
	float ScaleFactor = FMath::Clamp(100.f / (Distance / 100.f), 0.f, 32.f);

	// 위젯 크기 적용
	UMafiaPlayerNameActionGroupWidget* NameWidget = Cast<UMafiaPlayerNameActionGroupWidget>(PlayerNameComponent->GetWidget());
	if (IsValid(NameWidget))
	{
		NameWidget->SetFontSize(ScaleFactor, DeltaTime);
	}
	

}

void AMafiaSampleCharacter::ChangeColor(FLinearColor InColor)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		CharacterColor = InColor;
	}
}

void AMafiaSampleCharacter::ChangePlayerName(const FName& InNewPlayerName)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		PlayerName = InNewPlayerName;
	}
}

void AMafiaSampleCharacter::OnRepChangeColor()
{
	if (GetMesh()->GetMaterial(0))
	{
		GetMesh()->SetVectorParameterValueOnMaterials(TEXT("Tint"), FVector(CharacterColor));
	}

	UMafiaPlayerNameActionGroupWidget* NameWidget = Cast<UMafiaPlayerNameActionGroupWidget>(PlayerNameComponent->GetWidget());
	if (IsValid(NameWidget))
	{
		NameWidget->SetTextColor(CharacterColor);
	}
}

void AMafiaSampleCharacter::OnRepChangePlayerName()
{
	UMafiaPlayerNameActionGroupWidget* NameWidget = Cast<UMafiaPlayerNameActionGroupWidget>(PlayerNameComponent->GetWidget());
	if (IsValid(NameWidget))
	{
		NameWidget->SetPlayerName(PlayerName);
	}
}






//////////////////////////////////////////////////////////////////////////r
// Input

void AMafiaSampleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMafiaSampleCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMafiaSampleCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMafiaSampleCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMafiaSampleCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}