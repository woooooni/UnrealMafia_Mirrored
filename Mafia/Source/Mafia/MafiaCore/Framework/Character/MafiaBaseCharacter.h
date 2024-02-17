// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MafiaBaseCharacter.generated.h"

UCLASS(Abstract)
class MAFIA_API AMafiaBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMafiaBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	/** Always called immediately after spawning and reading in replicated properties */
	virtual void PostNetInit() override;
	
	virtual void BindDelegates() {}
	virtual void UnBindDelegates() {}

	virtual void OnRep_PlayerState() override;

	/** Event when this actor is clicked by the mouse when using the clickable interface. */
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
	/** Event when this actor is under the mouse when left mouse button is released while using the clickable interface. */
	virtual void NotifyActorOnReleased(FKey ButtonReleased = EKeys::LeftMouseButton) override;
};
