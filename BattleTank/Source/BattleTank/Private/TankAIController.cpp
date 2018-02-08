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

	if (!ThisTank)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get AI Tank."))
		return;
	}

	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get Player Tank."))
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
		// ThisTank->AimAt(PlayerTank->GetActorLocation());
		TankAimingComponent->AimAt(PlayerTank->GetActorLocation());

		// Fire if ready
		// ThisTank->Fire();
		TankAimingComponent->Fire();
	}
}



