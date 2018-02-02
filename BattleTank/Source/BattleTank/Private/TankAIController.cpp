// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ATankAIController called."))

	ATank* AITank = GetControlledTank();
	if (!AITank)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get AI Tank."))
		return;
	}

	ATank* PlayerTank = GetPlayerTank();
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get Player Tank."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Tank %s found the Player Tank: %s"), *(AITank->GetName()),*(PlayerTank->GetName()))
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO Move towards player

	// Aim towards player
	GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

	// Fire if ready
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
	{
		return nullptr;
	}
	else
	{
		return Cast<ATank>(PlayerPawn);
	}
}



