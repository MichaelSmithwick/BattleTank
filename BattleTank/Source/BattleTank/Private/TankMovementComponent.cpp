// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "TankTrack.h"
#include "Tank.h"

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

// Moves Tank forward/backward with input range -1 to 1
// Both tracks are driven equally forward or reverse determined by the sign of Throw
void UTankMovementComponent::IntendMoveForward(float Throw)
{
	// Multiplier is a blueprint settable parameter in Tank
	// used to scale the raw input value to this program
	if (LeftTrack && RightTrack)
	{
		ATank* Owner = Cast<ATank>(GetOwner());
		Throw *= Owner->GetForwardMultiplier();
		LeftTrack->DriveTrack(Throw);
		RightTrack->DriveTrack(Throw);
	}
}

// Moves Tank clockwise/counterclockwise with input range -1 to 1
// One track is driven forward and the other in reverse to accomplish the turn
void UTankMovementComponent::IntendTurnRight(float Throw)
{
	// Multiplier is a blueprint settable parameter in Tank
	// used to scale the raw input value to this program
	if (LeftTrack && RightTrack)
	{
		ATank* Owner = Cast<ATank>(GetOwner());
		Throw *= Owner->GetRightTurnMultiplier();
		LeftTrack->DriveTrack(Throw);
		RightTrack->DriveTrack(-Throw);
	}
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
