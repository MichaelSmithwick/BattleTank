// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "TankTrack.h"

// Given by the AI Parent Class. Indicates what vector must be taken to reach the target
// Obstacle avoidance is built into the Parent Class so the vector may not always point at the target.
void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// Get the direction the Tank is pointed in and convert both this vector and direction
	// vectors into Normal Vectors (i.e. where vector magnitude is equal to 1)
	FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

	// This calculated the 'X' vector quantity for forward/reverse movement
	float ForwardVelocity = FVector::DotProduct(TankForward, AIForwardIntention);

	// This calculates the 'Y' vector quantity for left/right movement -- replace by cross product
	// float RightVelocity = FMath::Sin(FMath::Acos(ForwardVelocity));

	// the Z vector is the magnitude of the angle between the forward direction and the intended direction
	// it is negative or positive as determined if target is left or right of the forward line
	float RightVelocity = FVector::CrossProduct(TankForward, AIForwardIntention).Z;

	// Move and/or turn the Tank
	IntendMoveForward(ForwardVelocity);
	IntendTurnRight(RightVelocity);
}

// moves tank
// if @Turn is false the Throw is applied equally to both Left and Right Tracks
// if @Turn is true the Throw is applied negatively to the Right Track causing the Tank to spin
void UTankMovementComponent::DriveTank(float Throw, bool Turn)
{
	if (LeftTrack && RightTrack)
	{
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(Throw*(Turn?-1.0:1.0));
	}
}

// Moves Tank forward/backward with input range -1 to 1
void UTankMovementComponent::IntendMoveForward(float Throw)
{
	// ForwardMultiplier is a blueprint setable parameter of TankTrack
	// for scaleing the input value
	DriveTank(ForwardMultiplier * Throw);
}

// Moves Tank clockwise/counterclockwise with input range -1 to 1
void UTankMovementComponent::IntendTurnRight(float Throw)
{
	// RightTurnMultiplier is a blueprint setable parameter of TankTrack
	// for scaleing the input value
	DriveTank(RightTurnMultiplier * Throw, true);
}

// Capture and store instantiated Track objects from blueprint
void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	if (LeftTrackToSet && RightTrackToSet)
	{
		LeftTrack = LeftTrackToSet;
		RightTrack = RightTrackToSet;
	}
}
