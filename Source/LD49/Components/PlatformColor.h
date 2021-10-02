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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Update();

	virtual void OnRegister();


public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	UMaterialInstance *Material;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
