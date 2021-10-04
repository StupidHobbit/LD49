// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LD49Character.generated.h"


class UPlatformColor;
class USphereComponent;
struct FTimerHandle;

UCLASS(config = Game)
class ALD49Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Colors", meta = (AllowPrivateAccess = "true"))
	UPlatformColor* CurrrentColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Colors", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlatformColor> NextColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Colors", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UPlatformColor>> ColorsToPickFrom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Colors", meta = (AllowPrivateAccess = "true"))
	float TimeToChangeColor;

	UPROPERTY(BlueprintReadOnly, Category = "Colors", meta = (AllowPrivateAccess = "true"))
	FTimerHandle TurnColorHandle;

	UPROPERTY(BlueprintReadOnly, Category = "CheckPoints", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> VisitedCheckPoints;
public:
	ALD49Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	bool IsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Win)
	bool HasWon = false;

	UFUNCTION(BlueprintCallable)
	void RestoreAtCheckpoint();
	
	UFUNCTION(BlueprintImplementableEvent)
	void AtCheckpoint();

	UFUNCTION(BlueprintImplementableEvent)
	void AtDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	bool IsDecaying;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void TurnColor();

	void UpdateNextColor();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void BeginPlay() override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaTime) override;
};


