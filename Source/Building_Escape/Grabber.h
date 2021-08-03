// Copyright Charles Walberg 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UGrabber : public UActorComponent {
	GENERATED_BODY()

public:	
	UGrabber(); // Sets default values for this component's properties
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; // Called every frame
	void FindPhysicsHandle();
	void Grab();
	void Release();
	void SetupInputComponent();
	FHitResult GetFirstPhysicsBodyInReach();

protected:
	virtual void BeginPlay() override; // Called when the game starts

private:
	UPROPERTY(EditAnywhere) float Reach = 100.0f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	FVector ReachVector;
};
