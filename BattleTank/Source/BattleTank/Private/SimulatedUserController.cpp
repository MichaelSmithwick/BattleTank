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


// Called every frame
void USimulatedUserController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	Left = UpdateController(Left);
	Right = UpdateController(Right);
}

float USimulatedUserController::LeftForward()
{
	Left += IncrementPerSecond;
	Left = FMath::Clamp<float>(Left, -1.0, 1.0);

	UE_LOG(LogTemp,Warning,TEXT("Left Forward: %f"), Left)

	return Left;
}

float USimulatedUserController::LeftReverse()
{
	Left -= IncrementPerSecond;
	Left = FMath::Clamp<float>(Left, -1.0, 1.0);

	UE_LOG(LogTemp, Warning, TEXT("Left Reverse: %f"), Left)

	return Left;
}

float USimulatedUserController::RightForward()
{
	Right += IncrementPerSecond;
	Right = FMath::Clamp<float>(Right, -1.0, 1.0);

	UE_LOG(LogTemp, Warning, TEXT("Right Forward: %f"),Right)
	return Right;
}

float USimulatedUserController::RightReverse()
{
	Right -= IncrementPerSecond;
	Right = FMath::Clamp<float>(Right, -1.0, 1.0);

	UE_LOG(LogTemp, Warning, TEXT("Right Reverse: %f"),Right)

	return Right;
}

float USimulatedUserController::ReturnLeft()
{
	return Left;
}

float USimulatedUserController::ReturnRight()
{
	return Right;
}

