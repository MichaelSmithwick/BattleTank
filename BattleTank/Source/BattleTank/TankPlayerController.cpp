// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankPlayerController.h"
#include "Engine/World.h"

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

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	ATank* MyTank = GetControlledTank();

	if (!MyTank)
	{
		UE_LOG(LogTemp,Error,TEXT("Unable to get Controlled Tank in AimTowardsCrosshair() function"))
		return;
	}

	FVector HitLocation;
//	UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *(HitLocation.ToString()))

	if (GetSightRayHitLocation(HitLocation))
	{
		UE_LOG(LogTemp,Warning,TEXT("New Hit Location: %s"),*(HitLocation.ToString()))
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	// HitLocation = FVector(1.0);
	// Find the crosshair
	// 'De-project' the screen position of the crosshair to a world direction
	// Linetrace along that look direction, and see what we hit (up to max range)

	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ScreenLocation = FVector2D(
		((float)ViewportSizeX)*CrossHairXLocation,
		((float)ViewportSizeY)*CrossHairYLocation
	);

	FVector LookDirection;
	if(GetLookDirection(ScreenLocation, LookDirection))
	{
		return GetLookVectorHitLocation(LookDirection,HitLocation);
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(const FVector2D& ScreenLocation, FVector& LookDirection) const
{
	FVector WorldLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(const FVector& LookDirection,FVector& HitLocation) const
{
	FHitResult OutHit;
	FVector Start=PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (LookDirection*LineTraceRange); // note vector functionality here!!
	/* -- This will work but the method used is more elegent
	FVector End=FVector(
		Start.X+(LookDirection.X*LineTraceRange),
		Start.Y+(LookDirection.Y*LineTraceRange),
		Start.Z+(LookDirection.Z*LineTraceRange)
	);
	*/
	FCollisionQueryParams Params;
	FCollisionResponseParams ResponseParam;

	if (GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECollisionChannel::ECC_Visibility, // hit anything that is visible
		Params,
		ResponseParam))
	{
		HitLocation = OutHit.Location;
		return true;
	}

	HitLocation = FVector(0.0);
	return false;
}

