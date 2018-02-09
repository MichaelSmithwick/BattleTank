// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankTrack.h"


void UTankTrack::DriveTrack(float Throttle)
{
	FString Name = GetName();

	// TODO clamp throttle value so player can't overdrive
	FVector ForceApplied = GetForwardVector()*Throttle*TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}


