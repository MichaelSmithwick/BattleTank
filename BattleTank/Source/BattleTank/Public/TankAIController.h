// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

	ATank* GetControlledTank() const;

	ATank* GetPlayerTank() const;
};
