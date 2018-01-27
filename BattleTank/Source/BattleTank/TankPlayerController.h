// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "BattleTank.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

//typedef int32 ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	ATank* GetControlledTank() const;
	
	
	
};
