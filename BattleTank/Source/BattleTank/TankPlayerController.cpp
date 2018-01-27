// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ATankPlayerController called."))

	ATank* MyTank = GetControlledTank();
	if (!MyTank)
	{
		UE_LOG(LogTemp,Error,TEXT("Unable to get a Tank."))
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Player found the following Tank: %s"),*(MyTank->GetName()))
	}
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}



