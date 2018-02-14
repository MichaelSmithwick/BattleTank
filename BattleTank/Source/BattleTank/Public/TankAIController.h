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
	virtual void SetPawn(APawn* InPawn) override;

protected:

	// How close to get to Player Tank in cm (80 meters/260 feet)
	UPROPERTY(EditAnywhere, Category = "Setup")
	float AcceptanceRadius = 8000;

	void MoveToActorResult(FPathFollowingRequestResult* PathMoveResult, EPathFollowingRequestResult::Type MoveResult, ATank * ThisTank);

private:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPossessedTankDeath();
};
