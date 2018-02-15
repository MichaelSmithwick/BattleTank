// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "TankAimingComponent.h"

// check for existence of Player Tank and Tank Aiming Component
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ATank* MyTank = GetControlledTank();
	if (!ensure(MyTank))
	{
		return;
	}

	UTankAimingComponent* TankAimingComponent = MyTank->FindComponentByClass<UTankAimingComponent>();
	if (ensure(TankAimingComponent))
	{
		FoundAimingComponent(TankAimingComponent);
	}
}

// move barrel and turret towards the aiming reticle firing solution
void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO It may be necessary to put a guard here to only call this when game is running
	AimTowardsCrosshair();
}

// This function is used by APlayerController to set the pawn being controlled, the Player Tank in this case
// Also set by this function is the broadcast receipt function used to receive the Tank OnDeath.Broadcast() broadcast.
void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);  // call Super to register Pawn with this Controller

	// if there is a live Pawn, cast it to Tank, and use ATank::OnDeath.AddUniqueDynamic() to set 
	// the class function to receive the broadcast (or in reality be called by the broadcast
	// function of the Tank)
	if (InPawn)
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank))
		{
			return;
		}

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

// responds to broadcast about player tanks death, i.e. this tanks death
void ATankPlayerController::OnPossessedTankDeath()
{
	StartSpectatingOnly();
}

// get pawn associated with this object and cast to ATank type
ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

// Get the location the gun sight is pointing at and pass it to the tank aiming system
void ATankPlayerController::AimTowardsCrosshair()
{
	ATank* MyTank = GetControlledTank();

	if (!MyTank)
	{
		// TODO Figure out why this is continually called even when game is not running
		// - if game is not running there will be no player tank
		return;
	}

	UTankAimingComponent* TankAimingComponent = MyTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(TankAimingComponent))
	{
		return;
	}

	FVector HitLocation;
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);

	if (bGotHitLocation)
	{
		// pass the HitLocation to the TankAimingComponent to handle moving the turret/barrel on target
		TankAimingComponent->AimAt(HitLocation);
	}
}

// Linetrace along that look direction, and see what we hit (up to max range)
// @return true if a HitLocation is detected, false if no HitLocation
// HitLocation value is indeterminate if false is returned
// HitLocation holds the X,Y,Z coordinates of the HitLocation if true is returned
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation)
{
	FVector2D ScreenLocation = CrosshairScreenLocation();
	FVector LookDirection;

	// Get the direction the player is looking in, i.e. LookDirection
	if(GetLookDirection(ScreenLocation, LookDirection))
	{
		// Return the hit location of the projectile along the LookDirection
		// or 0 and false if no hit location
		return GetLookVectorHitLocation(LookDirection,HitLocation);
	}

	return false;
}

// Get the X,Y coordinates of the Crosshair location in the viewport
// @CrossHairXLocation and @CrossHairYLocation are blueprint setable percentage values
FVector2D ATankPlayerController::CrosshairScreenLocation()
{
	// The aiming point is positioned statically in the viewport.
	// For example it may be positioned at 50% of the screen width and 30% of the screen height
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Calculate viewport coordinates by multiplying the percentage values (X%,Y%)
	// by the actual width and height of the viewport
	return FVector2D(
		((float)ViewportSizeX)*CrossHairXLocation,
		((float)ViewportSizeY)*CrossHairYLocation
	);
}

// get a unit vector pointing from player location through crosshairs location
// @ScreenLocation is the x,y coordinates of the crosshair on the screen
// @LookDirection is the returned unit vector
// @return false if unable, true if successful
bool ATankPlayerController::GetLookDirection(const FVector2D& ScreenLocation, FVector& LookDirection) const
{
	FVector WorldLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection);
}

// Check the vector from player through crosshair out to given range and see if any visible thing has been hit
// @LookDirection is a unit vector from the player position pointing through crosshair
// @HitLocation is returned as the location of the item that would be hit, or (0,0,0) if there is no object.
// @return false if no object to be hit, true if there is an object that would be hit.
bool ATankPlayerController::GetLookVectorHitLocation(const FVector& LookDirection,FVector& HitLocation) const
{
	FHitResult OutHit;
	FVector Start=PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (LookDirection*LineTraceRange); // note vector functionality here!!

	// calculate if there is any collision along path from Start to End
	if (GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECollisionChannel::ECC_Camera // hit anything that is visible changed from ECC_Visibility
	))
	{
		HitLocation = OutHit.Location;
		return true;
	}

	HitLocation = FVector(0.0);
	return false;
}

