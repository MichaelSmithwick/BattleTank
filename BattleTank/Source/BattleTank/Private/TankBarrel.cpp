// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankBarrel.h"

void UTankBarrel::ElevateBarrel(float DegreesPerSecond)
{
	FRotator BarrelRotator = GetForwardVector().Rotation();

	if (BarrelRotator.Pitch > MaxElevation)
	{
		BarrelRotator.Pitch = MaxElevation;
	}

	if (BarrelRotator.Pitch < MinElevation)
	{
		BarrelRotator.Pitch = MinElevation;
	}

	SetRelativeRotation(BarrelRotator);


	UE_LOG(LogTemp, Warning, TEXT("DegreesPerSecond: %f"), DegreesPerSecond)
}



