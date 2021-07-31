// Copyright of aradiuk

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabber();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float grabReachMultiplier_ = 100;

	UPhysicsHandleComponent* physicsHandle_ = nullptr;
	UInputComponent* inputComponent_ = nullptr;

	void Grab();
	void Release();
	void FindComponents();
	void SetupInputComponent();

	FHitResult GetFirstPhysicsBodyInReach() const;
};
