// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankAIController.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"

// check existence of Pawn for this object and existence of a human Player
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

void ATankAIController::MoveToActorResult(FPathFollowingRequestResult* PathMoveResult, EPathFollowingRequestResult::Type MoveResult, ATank * ThisTank)
{
	FString MoveResultStr;
	FString PathStr;

	UE_LOG(LogTemp, Warning, TEXT("%u"),PathMoveResult->MoveId.GetID());

	if(GetPathFollowingComponent() == nullptr)
	{
		PathStr = "PathFollowingComponent is null";
	}
	else
	{
		PathStr = "PathFollowingComponent is valid";
	}

	switch (MoveResult)
	{
	case EPathFollowingRequestResult::Type::Failed:
		MoveResultStr = "Movement Request Failed";
		break;
	case EPathFollowingRequestResult::Type::AlreadyAtGoal:
		MoveResultStr = "Already at the Goal";
		break;
	case EPathFollowingRequestResult::Type::RequestSuccessful:
		MoveResultStr = "Successful Move Request";
		break;
	default:
		MoveResultStr = "Undefined Move Request Result";
		break;
	}

	float Time = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("[%f] %s : Player Tank found: %s. (%s)"), Time, *(ThisTank->GetName()), *MoveResultStr, *PathStr)
}

// Called every frame
void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ATank* ThisTank = Cast<ATank>(GetPawn());
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// If there is a Player Tank in the world then head towards it and shoot at it
	if (PlayerTank)
	{
		// [ Part 1 ] Move towards player tank
		if (!ThisTank->IsGunOnly())
		{
			EPathFollowingRequestResult::Type MoveResult = MoveToActor(PlayerTank, AcceptanceRadius);
			// FPathFollowingRequestResult* PathMoveResult = (FPathFollowingRequestResult*)&MoveResult;
			// MoveToActorResult(PathMoveResult, MoveResult, ThisTank);
		}

		// [ Part 2 ] Get this tank's AimingComponent and Aim towards player
		UTankAimingComponent* TankAimingComponent = ThisTank->FindComponentByClass<UTankAimingComponent>();
		TankAimingComponent->AimAt(PlayerTank->GetActorLocation());

		// Fire if aiming solution locked
		if (TankAimingComponent->GetFiringStatus() == EFiringStatus::Locked)
		{
			TankAimingComponent->Fire();
		}
	}
}



