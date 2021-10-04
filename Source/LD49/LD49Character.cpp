// Copyright Epic Games, Inc. All Rights Reserved.

#include "LD49Character.h"
#include "CheckPoint.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "LD49/Components/PlatformColor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"

//////////////////////////////////////////////////////////////////////////
// AMyProject2Character
ALD49Character::ALD49Character()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	this->CurrentHealth = this->MaxHealth;

	if (this->VisitedCheckPoints.Num() == 0) {
		this->VisitedCheckPoints.Push(this->GetActorLocation());
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALD49Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALD49Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALD49Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALD49Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALD49Character::LookUpAtRate);
}


void ALD49Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALD49Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALD49Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALD49Character::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ALD49Character::BeginPlay()
{
	Super::BeginPlay();

	this->CurrentHealth = this->MaxHealth;

	this->TurnColor();
}


void ALD49Character::TurnColor()
{
	if (this->ColorsToPickFrom.Num() == 0) {
		return;
	}

	if (this->CurrrentColor != nullptr) {
		this->CurrrentColor->UnregisterComponent();
	}

	if (this->NextColor == nullptr) {
		UpdateNextColor();
	}
	this->CurrrentColor = NewObject<UPlatformColor>(this, this->NextColor, NAME_None, RF_Transient);
	this->CurrrentColor->RegisterComponent();

	UpdateNextColor();
	GetWorld()->GetTimerManager().SetTimer(
		this->TurnColorHandle,
		this, 
		&ALD49Character::TurnColor, 
		this->TimeToChangeColor, 
		false
	);
}

void ALD49Character::UpdateNextColor()
{
	auto i = FMath::RandRange(0, this->ColorsToPickFrom.Num() - 1);
	if (this->ColorsToPickFrom[i] == this->NextColor) {
		i = (i + 1) % this->ColorsToPickFrom.Num();
	}
	this->NextColor = this->ColorsToPickFrom[i];
}

// Called every frame
void ALD49Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IsDecaying = false;

	TArray<AActor*> OverlappingActors;
	this->GetOverlappingActors(OverlappingActors, nullptr);
	for (auto actor : OverlappingActors) {
		//UE_LOG(LogTemp, Warning, TEXT("%s\n"), *actor->GetName());
		auto PlatformColor = Cast<UPlatformColor>(actor->GetComponentByClass(UPlatformColor::StaticClass()));
		if (PlatformColor != nullptr) {
			if (PlatformColor->Name != this->CurrrentColor->Name) {
				this->CurrentHealth -= DeltaTime * this->DamageRate;
				IsDecaying = true;
				//UE_LOG(LogTemp, Warning, TEXT("%f\n"), this->CurrentHealth);
			}
		}

		auto checkpoint = Cast<ACheckPoint>(actor);
		if (checkpoint != nullptr) {
			this->VisitedCheckPoints.Push(checkpoint->GetTransform().GetLocation());
			checkpoint->Destroy();
			AtCheckpoint();
		}
	}
}

void ALD49Character::RestoreAtCheckpoint() {
	CurrentHealth = MaxHealth;

	if (this->VisitedCheckPoints.Num() != 0) {
		this->SetActorLocation(this->VisitedCheckPoints.Last());
	}
}