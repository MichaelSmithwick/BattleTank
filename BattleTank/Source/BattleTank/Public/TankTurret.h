// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurret.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = ("Collision"))
class BATTLETANK_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// the range is -1.0 to 1.0 from max down movement to max up movement
	void RotateTurret(float RelativeSpeed);

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxDegreesPerSecond = 15.0;
};
