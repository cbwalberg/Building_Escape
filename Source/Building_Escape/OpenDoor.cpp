// Copyright Charles Walberg 2021

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT // Denotes Out Parameters for Readability - no functional pupose, skipped by compiler

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	PrimaryComponentTick.bCanEverTick = true; // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features off to improve performance if you don't need them.
}

// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();

	CurrentRotation = InitialRotation = GetOwner() -> GetActorRotation();
	OpenAngle += InitialRotation.Yaw;
	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s is missing the Pressure Plate on the OpenDoor Component"), *GetOwner() -> GetName());
	AudioComponent = GetOwner() -> FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
		UE_LOG(LogTemp, Error, TEXT("%s is missing the Audio Component"), *GetOwner() -> GetName());	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (TotalMassOfActors() > RequiredMass) {
		OpenDoor(DeltaTime);
	} else if (GetWorld() -> GetTimeSeconds() - DoorOpenTime > DoorCloseDelay) {
		CloseDoor(DeltaTime);
	}
}

float UOpenDoor::TotalMassOfActors() const {
	float TotalMass = 0.0f;
	TArray<AActor*> ActorsOnPressurePlate;

	if (PressurePlate)
		PressurePlate -> GetOverlappingActors(OUT ActorsOnPressurePlate);

	for (AActor* Actor : ActorsOnPressurePlate) {
		TotalMass += Actor -> FindComponentByClass<UPrimitiveComponent>() -> GetMass();
	}

	return TotalMass;
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	CurrentRotation = FRotator(CurrentRotation.Pitch, FMath::FInterpTo(CurrentRotation.Yaw, OpenAngle, DeltaTime, 1.0f), CurrentRotation.Roll);
	GetOwner() -> SetActorRotation(CurrentRotation);
	DoorOpenTime = GetWorld() -> GetTimeSeconds();
	if (bDoorClosed && AudioComponent) {
		bDoorClosed = false;
		AudioComponent -> Play();
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	CurrentRotation = FRotator(CurrentRotation.Pitch, FMath::FInterpTo(CurrentRotation.Yaw, InitialRotation.Yaw, DeltaTime, 1.0f), CurrentRotation.Roll);
	GetOwner() -> SetActorRotation(CurrentRotation);
	if (!bDoorClosed && AudioComponent) {
		bDoorClosed = true;
		AudioComponent -> Play();
	}
}
