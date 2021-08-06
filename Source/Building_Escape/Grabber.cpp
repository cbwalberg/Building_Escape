// Copyright Charles Walberg 2021

#include "Grabber.h"
#include "GameFramework/PlayerController.h"

#define OUT // Denotes Out Parameters for Readability - no functional pupose, skipped by compiler

// Sets default values for this component's properties
UGrabber::UGrabber() {
	PrimaryComponentTick.bCanEverTick = true;	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features off to improve performance if you don't need them.
}

// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle -> GrabbedComponent)
		PhysicsHandle -> SetTargetLocation(GetPlayerReach());
}

void UGrabber::FindPhysicsHandle() {
	PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component on %s object"), *GetOwner() -> GetName());
}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner() -> FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent -> BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab() {
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (HitResult.GetActor() && PhysicsHandle) {
		PhysicsHandle -> GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
	}
}

void UGrabber::Release() {
	if (PhysicsHandle)
		PhysicsHandle -> ReleaseComponent();
}

// Returns the first Actor within reach with physics body
FHitResult UGrabber::GetFirstPhysicsBodyInReach() {	
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld() -> LineTraceSingleByObjectType(
		OUT Hit, 
		GetPlayerWorldPos(), 
		GetPlayerReach(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParams
	);

	return Hit;
}

// Returns the Player Reach
FVector UGrabber::GetPlayerReach() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	return GetPlayerWorldPos() + PlayerViewPointRotation.Vector()*Reach;
}

// Gets Player Position in World
FVector UGrabber::GetPlayerWorldPos() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}
