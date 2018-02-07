// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "TankTrack.h"


void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	FString Name = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s: MoveVelocity: %s, Maxspeed = %d"), *Name, *(MoveVelocity.ToString()), (int)bForceMaxSpeed)
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (LeftTrack && RightTrack)
	{
		LeftTrack->DriveTrack(Throw);
		RightTrack->DriveTrack(Throw);
	}
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (LeftTrack && RightTrack)
	{
		LeftTrack->DriveTrack(Throw);
		RightTrack->DriveTrack(-Throw);
	}
}

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	if (LeftTrackToSet && RightTrackToSet)
	{
		LeftTrack = LeftTrackToSet;
		RightTrack = RightTrackToSet;
	}
}
