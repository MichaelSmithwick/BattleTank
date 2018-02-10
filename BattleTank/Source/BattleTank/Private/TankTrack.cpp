// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankTrack.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

// turn off tick
UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// setup callback function for when the tank tracks hit something ... like the ground
void UTankTrack::BeginPlay()
{
	// TODO Make sure 'Simulation Generates Hit Events' is checked in Collision tab for TankTrack blueprint or this will not work
	// TODO Try to find out how to determine if this option is selected.
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

// call-back function called when tank track is hit or hits something
void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0.0;
}

// apply a stabilizing sideways force to TankTrack when turning
void UTankTrack::ApplySidewaysForce()
{
	// Dot product A.B = MA * MB * cos(T)  where A dot B = magnitude A x magnitude B x cos(angle between A and B)
	// Cross product AxB = MA * MB * sin(T) is the length of Z in the X Y Z universe - Z is orthogonal to X and Y which are orthogonal to each other
	// Dot product result is a float
	// Cross product result is a vector

	// get length of time slice in seconds -- used to calculate correction in cm/sec
	float DeltaTime = GetWorld()->DeltaTimeSeconds;

	// Get vector of movement orthogonal to forward path
	FVector RightVector = GetRightVector();

	// Calculate Slippage Speed
	float SlippageSpeed = FVector::DotProduct(RightVector, GetComponentVelocity());

	// Work-out the required acceleration this frame accel = speed/delta time -- multiply scalar by right vector for direction
	FVector CorrectionAcceleration = -(SlippageSpeed / DeltaTime) * RightVector; // negated to be reverse of slip acceleration vector

	// Get Tank
	UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	// Calculate sideways force (F = m a)
	FVector CorrectionForce = (TankRoot->GetMass() / 2.0) * CorrectionAcceleration; // mass divide by 2 because 2 tracks

	// Add the counter force in to stop side slip
	TankRoot->AddForce(CorrectionForce);
}

// set throttle level for this track
void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1.0, 1.0);
}

// moves tank by applying force based on Throttle value
void UTankTrack::DriveTrack()
{
	FVector ForceApplied = GetForwardVector()*CurrentThrottle*TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

