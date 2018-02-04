// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!GetPawn())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get AI Tank."))
		return;
	}

	if (!GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get Player Tank."))
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO Move towards player

	ATank* ThisTank = Cast<ATank>(GetPawn());
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!PlayerTank)
	{
		return;
	}

	// Aim towards player
	ThisTank->AimAt(PlayerTank->GetActorLocation());

	// Fire if ready
	ThisTank->Fire();
}

/* These methods are unused but may be recalled later
ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
	{
		return nullptr;
	}
	else
	{
		return Cast<ATank>(PlayerPawn);
	}
}
*/



