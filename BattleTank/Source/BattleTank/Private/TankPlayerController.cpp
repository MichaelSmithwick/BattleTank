// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "TankAimingComponent.h"

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

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO It may be necessary to put a guard here to only call this when game is running
	AimTowardsCrosshair();
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
		// TODO Figure out why this is continually called even when game is not running - if game is not running there will be no player tank
		// UE_LOG(LogTemp,Error,TEXT("Unable to get Controlled Tank in AimTowardsCrosshair() function"))
		return;
	}

	UTankAimingComponent* TankAimingComponent = MyTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(TankAimingComponent))
	{
		return;
	}

	FVector HitLocation;

	if (GetSightRayHitLocation(HitLocation))
	{
		TankAimingComponent->AimAt(HitLocation);
	}
}

// Linetrace along that look direction, and see what we hit (up to max range)
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation)
{
	FVector2D ScreenLocation = CrosshairScreenLocation();
	FVector LookDirection;

	if(GetLookDirection(ScreenLocation, LookDirection))
	{
		return GetLookVectorHitLocation(LookDirection,HitLocation);
	}

	return false;
}

// Get the X,Y coordinates of the Crosshair location in the viewport
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

	if (GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECollisionChannel::ECC_Visibility // hit anything that is visible
	))
	{
		HitLocation = OutHit.Location;
		return true;
	}

	HitLocation = FVector(0.0);
	return false;
}

