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



private:
	// TODO - Determine if these functions are needed for the AI Tank Controller
	void AimTowardsCrosshair();

	bool GetSightRayHitLocation(FVector & HitLocation) const;

	bool GetLookDirection(const FVector2D & ScreenLocation, FVector & LookDirection) const;

	bool GetLookVectorHitLocation(const FVector & LookDirection, FVector & HitLocation) const;

	/*
	ATank* GetControlledTank() const;
	ATank* GetPlayerTank() const;
	*/

	// How close to get to Player Tank in cm
	UPROPERTY(EditDefaultsOnly)
	float AcceptanceRadius = 300;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0; // 1 MILLION Centimeters!
};
