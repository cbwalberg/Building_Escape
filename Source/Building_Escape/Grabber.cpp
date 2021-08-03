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
	if (PhysicsHandle -> GrabbedComponent)
		PhysicsHandle -> SetTargetLocation(ReachVector);
}

void UGrabber::FindPhysicsHandle() {
	PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component on %s object"), *GetOwner() -> GetName());
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber Activated, %f"), GetWorld() -> GetTimeSeconds());
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	if (HitResult.GetActor()) {
		PhysicsHandle -> GrabComponentAtLocation(ComponentToGrab, NAME_None, ReachVector);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released, %f"), GetWorld() -> GetTimeSeconds());
	PhysicsHandle -> ReleaseComponent();
}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner() -> FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent -> BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("No InputComponent found on %s object"), *GetOwner() -> GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	ReachVector = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld() -> LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, ReachVector, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Hitting Actor: %s, %f"), *ActorHit -> GetName(), GetWorld() -> GetTimeSeconds());

	return Hit;
}
