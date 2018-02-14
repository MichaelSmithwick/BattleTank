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

	// Returns true if this object does not move
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsGunOnly();

private:
	// Prevents Path Finding if true, allows movement if false
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool GunOnly = false;
};
