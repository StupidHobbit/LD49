// Copyright Epic Games, Inc. All Rights Reserved.

#include "LD49GameMode.h"
#include "LD49HUD.h"
#include "LD49Character.h"
#include "UObject/ConstructorHelpers.h"
#include "LD49/LD49Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ALD49GameMode::ALD49GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ALD49HUD::StaticClass();
}

void ALD49GameMode::BeginPlay() {
	Super::BeginPlay();

	PlayerCharacter = Cast<ALD49Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerControllerRef = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	GameStart();
}
