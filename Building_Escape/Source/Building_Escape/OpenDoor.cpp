// Copyright of aradiuk


#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	initialYaw_ = GetOwner()->GetActorRotation().Yaw;
	currentYaw_ = initialYaw_;
	openYaw_ += initialYaw_;

	if (!pressurePlate_) {
		UE_LOG(LogTemp, Error, TEXT("Pressure plate is null for the %s."), *GetOwner()->GetName());
	}

	audioComponent_ = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!audioComponent_) {
		UE_LOG(LogTemp, Error, TEXT("Audio component was not found for %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Error, TEXT("triggerDoorOpen is %s and triggerDoorClose is %s."), triggerDoorOpenSound_ ? "T" : "F", triggerDoorCloseSound_ ? "T" : "F");

	if (TotalMassOnPressurePlate() > massToPressThePlate_) {
		OpenDoor(DeltaTime);
		doorLastOpenedTime_ = GetWorld()->GetTimeSeconds();
	} else if (GetWorld()->GetTimeSeconds() - doorLastOpenedTime_ > doorClosingDelay_) {
		CloseDoor(DeltaTime);	
	}
}

void UOpenDoor::RotateDoor(float DeltaTime, float yaw, bool toPlaySound)
{
	if (audioComponent_ && toPlaySound) {
		audioComponent_->Play();
	}

	FRotator newRotation = GetOwner()->GetActorRotation();
	currentYaw_ = FMath::Lerp(currentYaw_, yaw, doorSpeedMultiplier_ * DeltaTime);
	newRotation.Yaw = currentYaw_;

	GetOwner()->SetActorRotation(newRotation);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	RotateDoor(DeltaTime, openYaw_, triggerDoorOpenSound_);

	triggerDoorOpenSound_ = false;
	triggerDoorCloseSound_ = true;
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	RotateDoor(DeltaTime, initialYaw_, triggerDoorCloseSound_);

	triggerDoorCloseSound_ = false;
	triggerDoorOpenSound_ = true;
}

float UOpenDoor::TotalMassOnPressurePlate() const
{
	float totalMass = 0.f;
	if (!pressurePlate_) {
		return totalMass;
	}

	TArray<AActor*> overlappingActors;
	pressurePlate_->GetOverlappingActors(overlappingActors);

	for (const AActor* actorIt : overlappingActors) {
		if (actorIt) {
			const auto actorsPrimitiveComponent = actorIt->FindComponentByClass<UPrimitiveComponent>();
			if (actorsPrimitiveComponent) {
				totalMass += actorsPrimitiveComponent->GetMass();
			}
		}
	}

	return totalMass;
}