// Copyright of aradiuk

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::FindComponents()
{
	physicsHandle_ = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle_) {
		UE_LOG(LogTemp, Error, TEXT("Physics handle component was not found for %s"), *GetOwner()->GetName());
		return;
	}

	inputComponent_ = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!inputComponent_) {
		UE_LOG(LogTemp, Error, TEXT("Input component was not found for %s"), *GetOwner()->GetName());
		return;
	}
}

void UGrabber::SetupInputComponent()
{
	if (inputComponent_) {
		inputComponent_->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent_->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindComponents();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (physicsHandle_ && physicsHandle_->GrabbedComponent) {
		physicsHandle_->SetTargetLocation(GetPlayerGrabLocation());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector playerLocation;
	FRotator playerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerLocation, playerRotation);

	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(hit,
											GetPlayerLocation(),
											GetPlayerGrabLocation(),
											FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
											FCollisionQueryParams(FName(TEXT("")), false, GetOwner()));

	AActor* actorHit = hit.GetActor();
	if (actorHit) {
		UE_LOG(LogTemp, Warning, TEXT("We've hit an actor named %s"), *actorHit->GetName());
	}

	return hit;
}

void UGrabber::Grab()
{
	FHitResult hitResult = GetFirstPhysicsBodyInReach();

	if (physicsHandle_ && hitResult.GetActor()) {
		physicsHandle_->GrabComponentAtLocation(hitResult.GetComponent(), NAME_None, GetPlayerGrabLocation());
	}
}

void UGrabber::Release()
{
	if (physicsHandle_ && physicsHandle_->GrabbedComponent) {
		physicsHandle_->ReleaseComponent();
	}
}

FVector UGrabber::GetPlayerGrabLocation() const
{
	FVector playerLocation;
	FRotator playerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerLocation, playerRotation);

	return playerLocation + playerRotation.Vector() * grabReachMultiplier_;
}

FVector UGrabber::GetPlayerLocation() const
{
	FVector playerLocation;
	FRotator playerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerLocation, playerRotation);

	return playerLocation;
}