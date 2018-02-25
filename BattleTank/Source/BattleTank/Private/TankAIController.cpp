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

// This method is used by AAIController to set the Pawn being controlled, the Tank in this case.
// Also set by this function is the broadcast receipt function used to receive the Tank OnDeath.Broadcast() broadcast.
void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);  // call Super to register Pawn with this Controller

	// if there is a live Pawn, cast it to Tank, and use ATank::OnDeath.AddUniqueDynamic() to set 
	// the class function to receive the broadcast (or in reality be called by the broadcast
	// function of the Tank)
	if(InPawn)
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank))
		{
			return;
		}

		// Subscribe our local method to the controlled Tank's death event broadcast (OnDeath.Broadcast())
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

// This function is called by the Tank owned by the AIObject, i.e. the instantiated Tank this controller is controlling.
// This function is called through the broadcast method of ATank::OnDeath.Broadcast() from the Tank controlled by this controller.
// This function is added to this controllers controlled Tank through the OnDeath.AddUniqueDynamic() method.
void ATankAIController::OnPossessedTankDeath()
{
	if(GetPawn())
	{
		GetPawn()->DetachFromControllerPendingDestroy();
	}
}

void ATankAIController::MoveToActorResult(FPathFollowingRequestResult* PathMoveResult, EPathFollowingRequestResult::Type MoveResult, ATank * ThisTank)
{
	FString MoveResultStr;
	FString PathStr;

	// UE_LOG(LogTemp, Warning, TEXT("%u"),PathMoveResult->MoveId.GetID());

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

	// float Time = GetWorld()->GetTimeSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("[%f] %s : Player Tank found: %s. (%s)"), Time, *(ThisTank->GetName()), *MoveResultStr, *PathStr)
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
		if (ThisTank->IsMoveAble())
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



