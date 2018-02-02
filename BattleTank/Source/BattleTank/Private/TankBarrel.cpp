// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankBarrel.h"

// moves barrel up or down. The movement is a combination of the RelativeSpeed multiplying
// the MaxDegreesPerSecond
// @RelativeSpeed adjusts movement rate. Range -1.0<=RelativeSpeed<=1.0
void UTankBarrel::ElevateBarrel(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0, 1.0);

	float ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float RawNewElevation = RelativeRotation.Pitch + ElevationChange;

	float ValidElevation = FMath::Clamp<float>(RawNewElevation, MinElevation, MaxElevation);

	SetRelativeRotation(FRotator(ValidElevation,0.0,0.0));
}



