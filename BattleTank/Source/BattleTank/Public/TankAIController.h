// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:		
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:

	// How close to get to Player Tank in cm
	UPROPERTY(EditAnywhere, Category = "Setup")
	float AcceptanceRadius = 8000;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0; // 1 MILLION Centimeters!

private:
	// TODO - Determine if these functions are needed for the AI Tank Controller
	void AimTowardsCrosshair();

	bool GetSightRayHitLocation(FVector & HitLocation) const;

	bool GetLookDirection(const FVector2D & ScreenLocation, FVector & LookDirection) const;

	bool GetLookVectorHitLocation(const FVector & LookDirection, FVector & HitLocation) const;

};
