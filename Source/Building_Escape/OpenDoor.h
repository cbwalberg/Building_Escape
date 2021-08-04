// Copyright Charles Walberg 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent {
	GENERATED_BODY()

public:	
	UOpenDoor(); // Sets default values for this component's properties
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override; // Called when the game starts

private:
	UPROPERTY(EditAnywhere) float RequiredMass = 60.0f;
	UPROPERTY(EditAnywhere) float OpenAngle = 90.0f;
	UPROPERTY(EditAnywhere) float DoorOpenTime = 0.0f;
	UPROPERTY(EditAnywhere) float DoorOpenSpeed = 1.0f;
	UPROPERTY(EditAnywhere) float DoorCloseDelay = 2.0f;
	UPROPERTY(EditAnywhere) float DoorCloseSpeed = 1.0f;
	UPROPERTY(EditAnywhere) ATriggerVolume* PressurePlate;
	UPROPERTY(EditAnywhere) AActor* ActorThatOpens;

	FRotator InitialRotation, CurrentRotation;

	float TotalMassOfActors() const;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
};
