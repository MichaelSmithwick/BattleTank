// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = ("Collision"))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// the range is -1.0 to 1.0 from max down movement to max up movement
	void ElevateBarrel(float RelativeSpeed);

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxDegreesPerSecond = 6.0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxElevation = 30.0; // degrees up

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MinElevation = -2.0; // degrees down	
};
