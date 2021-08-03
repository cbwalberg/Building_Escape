// Copyright Charles Walberg 2021

#include "OpenDoor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();
	CurrentRotation = InitialRotation = GetOwner() -> GetActorRotation();
	OpenAngle += InitialRotation.Yaw;

	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate is null in OpenDoor component of %s"), *GetOwner() -> GetName());

	ActorThatOpens = GetWorld() -> GetFirstPlayerController() -> GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (PressurePlate && PressurePlate -> IsOverlappingActor(ActorThatOpens)) {
		OpenDoor(DeltaTime);
		DoorOpenTime = GetWorld() -> GetTimeSeconds();
	} else if (GetWorld() -> GetTimeSeconds() - DoorOpenTime > DoorCloseDelay) {
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	CurrentRotation = FRotator(CurrentRotation.Pitch, FMath::FInterpTo(CurrentRotation.Yaw, OpenAngle, DeltaTime, 1.0f), CurrentRotation.Roll);
	GetOwner() -> SetActorRotation(CurrentRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	CurrentRotation = FRotator(CurrentRotation.Pitch, FMath::FInterpTo(CurrentRotation.Yaw, InitialRotation.Yaw, DeltaTime, 1.0f), CurrentRotation.Roll);
	GetOwner() -> SetActorRotation(CurrentRotation);
}
