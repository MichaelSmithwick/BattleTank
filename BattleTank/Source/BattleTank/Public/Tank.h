// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// getter for ForwardMutltiplier
	float GetForwardMultiplier();

	// getter for RightTurnMultiplier
	float GetRightTurnMultiplier();

protected:
private:
	// scales the forward input value for controlability
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ForwardMultiplier = 0.125;

	// scales the right turn input value for controlability
	UPROPERTY(EditAnywhere, Category = "Setup")
	float RightTurnMultiplier = 0.125;
};
