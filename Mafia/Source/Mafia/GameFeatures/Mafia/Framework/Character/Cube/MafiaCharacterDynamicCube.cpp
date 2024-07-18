// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Mafia/Framework/Character/Cube/MafiaCharacterDynamicCube.h"

// Sets default values
AMafiaCharacterDynamicCube::AMafiaCharacterDynamicCube()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMafiaCharacterDynamicCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMafiaCharacterDynamicCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMafiaCharacterDynamicCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

