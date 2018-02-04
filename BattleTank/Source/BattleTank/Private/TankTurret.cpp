// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankTurret.h"
#include "Engine/World.h"

// moves turret arround. The movement is a combination of the RelativeSpeed multiplying
// the MaxDegreesPerSecond
// @RelativeSpeed adjusts movement rate. Range -1.0<=RelativeSpeed<=1.0
void UTankTurret::RotateTurret(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0, 1.0);

	float RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float ValidRotation = RelativeRotation.Yaw + RotationChange;

	SetRelativeRotation(FRotator(RelativeRotation.Pitch, ValidRotation, RelativeRotation.Roll));
}



