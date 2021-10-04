// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformColor.h"
#include "Components/MeshComponent.h"


// Sets default values for this component's properties
UPlatformColor::UPlatformColor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Update();

}


// Called when the game starts
void UPlatformColor::BeginPlay()
{
	Super::BeginPlay();

	Update();
}


// Called every frame
void UPlatformColor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlatformColor::Update() {
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Name);
	auto owner = GetOwner();
	if (owner == nullptr) return;

	auto mesh = Cast<UMeshComponent>(owner->GetComponentByClass(UMeshComponent::StaticClass()));
	if (mesh != nullptr && Material != nullptr) {
		mesh->SetMaterial(0, Material);
	}
}

void UPlatformColor::OnRegister()
{
	Super::OnRegister();
	Update();
}
