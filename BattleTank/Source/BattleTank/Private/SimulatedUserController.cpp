// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "SimulatedUserController.h"
#include "Engine/World.h"
#include "TankTrack.h"


// Sets default values for this component's properties
USimulatedUserController::USimulatedUserController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimulatedUserController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void USimulatedUserController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	/* Remove this for now
	if (RightTrack && LeftTrack)
	{
		RightTrack->DriveTrack(Right);
		LeftTrack->DriveTrack(Left);
	}
	*/
}

// Capture left and right track objects
void USimulatedUserController::Initialize(UTankTrack * TrackR, UTankTrack * TrackL)
{
	RightTrack = TrackR;
	LeftTrack = TrackL;
}

// calculate the delta to use for movement during the tick phase
float USimulatedUserController::MovementIncrement()
{
	// to sync with game time use this formula
	// float Increment = IncrementPerSecond * GetWorld()->DeltaTimeSeconds;
	return IncrementPerSecond;
}

float USimulatedUserController::LeftForward()
{
	Left += MovementIncrement();
	Left = FMath::Clamp<float>(Left, -1.0, 1.0);
	return Left;
}

float USimulatedUserController::LeftReverse()
{
	Left -= MovementIncrement();
	Left = FMath::Clamp<float>(Left, -1.0, 1.0);
	return Left;
}

float USimulatedUserController::RightForward()
{
	Right += MovementIncrement();
	Right = FMath::Clamp<float>(Right, -1.0, 1.0);
	return Right;
}

float USimulatedUserController::RightReverse()
{
	Right -= MovementIncrement();
	Right = FMath::Clamp<float>(Right, -1.0, 1.0);
	return Right;
}

bool USimulatedUserController::CmdLeftForward()
{
	CmdLFwd = true;
	CmdLRev = false;
	LeftForward();
	return true;
}

bool USimulatedUserController::CmdLeftReverse()
{
	CmdLFwd = false;
	CmdLRev = true;
	LeftReverse();
	return true;
}

bool USimulatedUserController::CmdRightForward()
{
	CmdRFwd = true;
	CmdRRev = false;
	RightForward();
	return true;
}

bool USimulatedUserController::CmdRightReverse()
{
	CmdRFwd = false;
	CmdRRev = true;
	RightReverse();
	return true;
}

void USimulatedUserController::CmdLeftStop()
{
	CmdLFwd = false;
	CmdLRev = false;
	Left = 0.0;
}

void USimulatedUserController::CmdRightStop()
{
	CmdRFwd = false;
	CmdRRev = false;
	Right = 0.0;
}

void USimulatedUserController::CmdForward()
{
	CmdRightForward();
	CmdLeftForward();
}

void USimulatedUserController::CmdReverse()
{
	CmdRightReverse();
	CmdLeftReverse();
}

void USimulatedUserController::CmdLeft()
{
	CmdLeftReverse();
	CmdRightForward();
}

void USimulatedUserController::CmdRight()
{
	CmdRightReverse();
	CmdLeftForward();
}

void USimulatedUserController::CmdStop()
{
	CmdRightStop();
	CmdLeftStop();
}


float USimulatedUserController::ReturnLeft()
{
	return Left;
}

float USimulatedUserController::ReturnRight()
{
	return Right;
}

void USimulatedUserController::ReturnRightAndLeft(float& RightValue, float& LeftValue) const
{
	RightValue = Right;
	LeftValue = Left;
}