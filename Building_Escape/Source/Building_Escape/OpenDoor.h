// Copyright of aradiuk

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void RotateDoor(float DeltaTime, float yaw);

private:
	float initialYaw_ = 0.f;
	float currentYaw_= 0.f;
	float doorLastOpenedTime_ = 0.f;

	UPROPERTY(EditAnywhere)
	float openYaw_ = -90.f;

	UPROPERTY(EditAnywhere)
	float doorClosingDelay_ = 0.5f;

	UPROPERTY(EditAnywhere)
	float doorSpeedMultiplier_ = 2.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressurePlate_;

	UPROPERTY(EditAnywhere)
	AActor* thePressor_; // The one that presses.
};
