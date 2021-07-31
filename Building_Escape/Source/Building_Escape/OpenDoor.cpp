// Copyright of aradiuk


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

	thePressor_ = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!thePressor_) {
		UE_LOG(LogTemp, Error, TEXT("The actor that should press the plate is null for the %s."), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (thePressor_ && pressurePlate_) {
		if (pressurePlate_->IsOverlappingActor(thePressor_)) {
			OpenDoor(DeltaTime);
			doorLastOpenedTime_ = GetWorld()->GetTimeSeconds();
		} else {
			if (GetWorld()->GetTimeSeconds() - doorLastOpenedTime_ > doorClosingDelay_) {
				CloseDoor(DeltaTime);
			}
		}
	}
}

void UOpenDoor::RotateDoor(float DeltaTime, float yaw)
{
	FRotator newRotation = GetOwner()->GetActorRotation();
	currentYaw_ = FMath::Lerp(currentYaw_, yaw, doorSpeedMultiplier_ * DeltaTime);
	newRotation.Yaw = currentYaw_;

	GetOwner()->SetActorRotation(newRotation);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	RotateDoor(DeltaTime, openYaw_);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	RotateDoor(DeltaTime, initialYaw_);
}
