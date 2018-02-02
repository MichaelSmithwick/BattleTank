// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "BattleTank.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TankPlayerController.generated.h"

//typedef int32 ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:

	ATank* GetControlledTank() const;

	// Start the tank moving the barrel so that a shot would hit where
	// the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(FVector & HitLocation);

	// Get X,Y viewport coordinates for Crosshair screen location
	FVector2D CrosshairScreenLocation();

	bool GetLookDirection(const FVector2D& ScreenLocation, FVector & LookDirection) const;

	bool GetLookVectorHitLocation(const FVector& LookDirection, FVector& HitLocation) const;

	// Crosshair horizontal position on screen - range 0.0 to 1.0
	// Origin in upper left hand corner
	UPROPERTY(EditAnywhere)
	float CrossHairXLocation = 0.5000;

	// Crosshair vertical position on screen - range 0.0 to 1.0
	// Origin in upper left hand corner
	UPROPERTY(EditAnywhere)
	float CrossHairYLocation = 0.3333;

	// TODO Make this parameter to be kilometers and scale programatically
	//      to be centimeters - 1 Million cm is crazy when most calculations
	//      are going to be in kilometers!
	// distance in centimeters
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.0; // 1 MILLION Centimeters!
};
