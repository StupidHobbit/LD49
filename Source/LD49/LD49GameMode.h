// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LD49GameMode.generated.h"


class ALD49Character;
class APlayerController;

UCLASS(minimalapi)
class ALD49GameMode : public AGameModeBase
{
	GENERATED_BODY()

	APlayerController* PlayerControllerRef;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ALD49Character* PlayerCharacter;

protected:
	virtual void BeginPlay() override;


public:
	ALD49GameMode();

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();

};



