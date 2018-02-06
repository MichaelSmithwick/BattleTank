// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "SimulatedUserController.h"
#include "Engine/World.h"


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
	// Left = UpdateController(Left);
	// Right = UpdateController(Right);

	if (CmdLFwd && Left < 1.0)
	{
		if (LeftForward() >= 1.0)
		{
			CmdLFwd = false;
		}

	}
	if (CmdLRev && Left > -1.0)
	{
		if (LeftReverse() <= -1.0)
		{
			CmdLRev = false;
		}

	}

	if (CmdRFwd && Right < 1.0)
	{
		if (RightForward() >= 1.0)
		{
			CmdRFwd = false;
		}

	}
	if (CmdRRev && Right > -1.0)
	{
		if (RightReverse() <= -1.0)
		{
			CmdRRev = false;
		}

	}

	// update blueprint
	ReturnLeft();
	ReturnRight();
}

// Incrementally returns controller value to zero position
float USimulatedUserController::UpdateController(float CurrentValue)
{
	float Increment = IncrementPerSecond * GetWorld()->DeltaTimeSeconds;

	if (CurrentValue > 0.0)
	{
		CurrentValue -= Increment;
	}
	if (CurrentValue < 0.0)
	{
		CurrentValue += Increment;
	}
	if (FMath::Abs<float>(CurrentValue) < Increment)
	{
		CurrentValue = 0.0;
	}

	return CurrentValue;
}

float USimulatedUserController::LeftForward()
{
	Left += IncrementPerSecond * GetWorld()->DeltaTimeSeconds;
	Left = FMath::Clamp<float>(Left, -1.0, 1.0);

	UE_LOG(LogTemp,Warning,TEXT("Left Forward: %f"), Left)

	return Left;
}

float USimulatedUserController::LeftReverse()
{
	Left -= IncrementPerSecond * GetWorld()->DeltaTimeSeconds;
	Left = FMath::Clamp<float>(Left, -1.0, 1.0);

	UE_LOG(LogTemp, Warning, TEXT("Left Reverse: %f"), Left)

	return Left;
}

float USimulatedUserController::RightForward()
{
	Right += IncrementPerSecond * GetWorld()->DeltaTimeSeconds;
	Right = FMath::Clamp<float>(Right, -1.0, 1.0);

	UE_LOG(LogTemp, Warning, TEXT("Right Forward: %f"),Right)
	return Right;
}

float USimulatedUserController::RightReverse()
{
	Right -= IncrementPerSecond * GetWorld()->DeltaTimeSeconds;
	Right = FMath::Clamp<float>(Right, -1.0, 1.0);

	UE_LOG(LogTemp, Warning, TEXT("Right Reverse: %f"),Right)

	return Right;
}

bool USimulatedUserController::CMDLeftForward()
{
	CmdLFwd = true;
	CmdLRev = false;
	return true;
}

bool USimulatedUserController::CMDLeftReverse()
{
	CmdLFwd = false;
	CmdLRev = true;
	return true;
}

bool USimulatedUserController::CMDRightForward()
{
	CmdRFwd = true;
	CmdRRev = false;
	return true;
}

bool USimulatedUserController::CMDRightReverse()
{
	CmdRFwd = false;
	CmdRRev = true;
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

float USimulatedUserController::ReturnLeft()
{
	return Left;
}

float USimulatedUserController::ReturnRight()
{
	return Right;
}

