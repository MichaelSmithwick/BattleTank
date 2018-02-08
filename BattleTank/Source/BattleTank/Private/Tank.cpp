// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankTrack.h"
#include "Engine/World.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

float ATank::GetForwardMultiplier()
{
	return ForwardMultiplier;
}

float ATank::GetRightTurnMultiplier()
{
	return RightTurnMultiplier;
}

