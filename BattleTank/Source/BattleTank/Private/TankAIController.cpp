// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankAIController.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* ThisTank = GetPawn();
	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!ensure(ThisTank))
	{
		return;
	}

	if (!ensure(PlayerTank))
	{
		return;
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ATank* ThisTank = Cast<ATank>(GetPawn());
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerTank)
	{
		// Move towards player tank
		MoveToActor(PlayerTank, AcceptanceRadius);

		ATank* ThisTank = Cast<ATank>(GetPawn());
		UTankAimingComponent* TankAimingComponent = ThisTank->FindComponentByClass<UTankAimingComponent>();

		// Aim towards player
		TankAimingComponent->AimAt(PlayerTank->GetActorLocation());

		if (TankAimingComponent->GetFiringStatus() == EFiringStatus::Locked)
		{
			// Fire if ready
			TankAimingComponent->Fire();
		}
	}
}



