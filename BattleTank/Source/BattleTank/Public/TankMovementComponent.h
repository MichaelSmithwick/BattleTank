// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 * Responsible for driving the Tank tracks
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:

	// Blueprint input catch method
	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveForward(float Throw);

	// Blueprint input catch method
	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendTurnRight(float Throw);

	// Tank tracks are entered in the blueprint
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet);

protected:

	// scales the forward input value for controlability
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ForwardMultiplier = 0.35;

	// scales the right turn input value for controlability
	UPROPERTY(EditAnywhere, Category = "Setup")
	float RightTurnMultiplier = 1.0;

private:
	// Overridden AI method called to move object -- drives tracks in this case
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	// move tank through the space
	void DriveTank(float Throw, bool Turn=false);

	// Pointer to TankTrack objects from Initialize() function - used to drive Tank
	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;
};
