// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlatformColor.generated.h"


class UMaterialInstance;


UENUM(BlueprintType)
enum class ColorType : uint8 {
	TREE = 0 UMETA(DisplayName = "Tree"),
	ROCK = 1 UMETA(DisplayName = "Rock"),
	METAL = 2 UMETA(DisplayName = "Metal"),
	GREEN = 3 UMETA(DisplayName = "Green"),
	RED = 4 UMETA(DisplayName = "Red"),
	BLUE UMETA(DisplayName = "Blue")
};


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LD49_API UPlatformColor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlatformColor();
	void Update();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister();


public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	UMaterialInstance *Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	UMaterialInstance* MaterialForUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	float DamagePerSecond = 10;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
