// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ATankPlayerController called."))

	ATank* MyTank = GetControlledTank();
	if (!MyTank)
	{
		UE_LOG(LogTemp,Error,TEXT("Unable to get a Tank."))
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Player found the following Tank: %s"),*(MyTank->GetName()))
	}
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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
		UE_LOG(LogTemp,Error,TEXT("Unable to get Controlled Tank in AimTowardsCrosshair() function"))
		return;
	}

	FVector HitLocation;

	if (GetSightRayHitLocation(HitLocation))
	{
		MyTank->AimAt(HitLocation);
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
