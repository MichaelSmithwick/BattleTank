// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "TankTrack.h"


void UTankMovementComponent::IntendMoveForward(float Throw)
{
	float Time = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("%f:Move forward: %f"), Time, Throw)

	LeftTrack->DriveTrack(Throw);
	RightTrack->DriveTrack(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	float Time = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("%f:Move forward: %f"), Time, Throw)

	LeftTrack->DriveTrack(Throw);
	RightTrack->DriveTrack(-Throw);
}

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	if (LeftTrackToSet && RightTrackToSet)
	{
		LeftTrack = LeftTrackToSet;
		RightTrack = RightTrackToSet;
	}
}
