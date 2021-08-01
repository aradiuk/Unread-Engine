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

	if (physicsHandle_->GrabbedComponent) {
		FVector playerLocation;
		FRotator playerRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerLocation, playerRotation);

		FVector lineTraceEnd = playerLocation + playerRotation.Vector() * grabReachMultiplier_;

		physicsHandle_->SetTargetLocation(lineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector playerLocation;
	FRotator playerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerLocation, playerRotation);

	FVector lineTraceEnd = playerLocation + playerRotation.Vector() * grabReachMultiplier_;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(hit,
											playerLocation,
											lineTraceEnd,
											FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
											traceParams);

	AActor* actorHit = hit.GetActor();
	if (actorHit) {
		UE_LOG(LogTemp, Warning, TEXT("We've hit an actor named %s"), *actorHit->GetName());
	}

	return hit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Function 'Grab' was called by pressing a button."));
	FHitResult hitResult = GetFirstPhysicsBodyInReach();

	if (physicsHandle_ && hitResult.GetActor()) {
		FVector playerLocation;
		FRotator playerRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerLocation, playerRotation);

		FVector lineTraceEnd = playerLocation + playerRotation.Vector() * grabReachMultiplier_;

		physicsHandle_->GrabComponentAtLocation(hitResult.GetComponent(), NAME_None, lineTraceEnd);
	}
}

void UGrabber::Release()
{
	if (physicsHandle_ && physicsHandle_->GrabbedComponent) {
		physicsHandle_->ReleaseComponent();
	}
	UE_LOG(LogTemp, Warning, TEXT("Function 'Release' was called by releasing a button."));
}