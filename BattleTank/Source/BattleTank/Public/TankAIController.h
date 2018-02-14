// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

/**
 * Controller for the computer controlled Tank
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:		
	virtual void BeginPlay() override;
	void MoveToActorResult(FPathFollowingRequestResult* PathMoveResult, EPathFollowingRequestResult::Type MoveResult, ATank * ThisTank);
	virtual void Tick(float DeltaSeconds) override;

protected:

	// How close to get to Player Tank in cm (80 meters/260 feet)
	UPROPERTY(EditAnywhere, Category = "Setup")
	float AcceptanceRadius = 8000;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LineTraceRange = 1000000.0; // 1 MILLION Centimeters!
};
