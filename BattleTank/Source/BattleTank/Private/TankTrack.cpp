// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankTrack.h"
#include "Engine/World.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// called every Tick
void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Dot product A.B = MA * MB * cos(T)  where A dot B = magnitude A x magnitude B x cos(angle between A and B)
	// Cross product AxB = MA * MB * sin(T) is the length of Z in the X Y Z universe - Z is orthogonal to X and Y which are orthogonal to each other
	// Dot product result is a float
	// Cross product result is a vector

	// Get vector of movement orthogonal to forward path
	FVector RightVector = GetRightVector();

	// Calculate Slippage Speed
	float SlippageSpeed = FVector::DotProduct(RightVector, GetComponentVelocity());

	// Work-out the required acceleration this frame accel = speed/delta time -- multiply scalar by right vector for direction
	FVector CorrectionAcceleration = -(SlippageSpeed / DeltaTime) * RightVector;

	// Calculate and apply sideways force (F = m a)
	// Get Tank for mass and to apply force
	UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	FVector CorrectionForce = (TankRoot->GetMass()/2.0)*CorrectionAcceleration; // mass divide by 2 because 2 tracks
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::DriveTrack(float Throttle)
{
	//FString Name = GetName();
	//UE_LOG(LogTemp,Warning,TEXT("%s : DriveTrack : %f"),*Name,Throttle)

	// TODO clamp throttle value so player can't overdrive
	FVector ForceApplied = GetForwardVector()*Throttle*TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::BeginPlay()
{
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	float Time = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp,Warning,TEXT("%f ELEPHANT: TankTrack Hit"),Time)
}


