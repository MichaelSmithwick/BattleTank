// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimulatedUserController.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API USimulatedUserController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimulatedUserController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Setup)
	float LeftForward();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float LeftReverse();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float RightForward();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float RightReverse();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float ReturnLeft();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float ReturnRight();

	UFUNCTION(BlueprintImplementableEvent, Category = Other)
	void UpdateController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	
	float Left = 0.0;

	float Right = 0.0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float IncrementPerSecond = 0.1;


	float UpdateController(float CurrentValue);
	
};
