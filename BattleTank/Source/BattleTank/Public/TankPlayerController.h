// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "TankPlayerController.generated.h"

class ATank;

/**
 * Controls aiming for the Player Tank
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// need this function in order to register for the Player Tank Death Broadcast
	virtual void SetPawn(APawn* InPawn) override;

protected: // blueprints can access protected members and functions

	// Get the Tank being controlled by this object
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ATank * GetControlledTank() const;

	// creates the event that will setup the Player_UI Blueprint
	// this function is declared but not defined, UE4 defines and links it to blueprint
	// this function is called in BeginPlay where the UTankAimingComponent object is 
	// retreived from the Tank and passed to it.
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingComponentReference);

private:
	// When Tank Player Tank Dies This function is called as part of that broadcast
	UFUNCTION()
	void OnPossessedTankDeath();

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
	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5000;

	// Crosshair vertical position on screen - range 0.0 to 1.0
	// Origin in upper left hand corner
	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.3333;

	// TODO Make this parameter to be kilometers and scale programatically
	//      to be centimeters - 1 Million cm is crazy when most calculations
	//      are going to be in kilometers!
	// distance in centimeters
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0; // 1 MILLION Centimeters!
};
