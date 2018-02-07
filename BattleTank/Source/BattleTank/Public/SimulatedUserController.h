// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimulatedUserController.generated.h"

class UTankTrack;

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
	void Initialize(UTankTrack* TrackR, UTankTrack* TrackL);

	UFUNCTION(BlueprintCallable, Category = Setup)
	float LeftForward();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float LeftReverse();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float RightForward();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float RightReverse();

	UFUNCTION(BlueprintCallable, Category = Setup)
	bool CmdLeftForward();

	UFUNCTION(BlueprintCallable, Category = Setup)
	bool CmdLeftReverse();

	UFUNCTION(BlueprintCallable, Category = Setup)
	bool CmdRightForward();

	UFUNCTION(BlueprintCallable, Category = Setup)
	bool CmdRightReverse();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void CmdLeftStop();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void CmdRightStop();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void CmdForward();
	
	UFUNCTION(BlueprintCallable, Category = Setup)
	void CmdReverse();
	
	UFUNCTION(BlueprintCallable, Category = Setup)
	void CmdLeft();
	
	UFUNCTION(BlueprintCallable, Category = Setup)
	void CmdRight();
	
	UFUNCTION(BlueprintCallable, Category = Setup)
	void CmdStop();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void ReturnRightAndLeft(float& RightValue, float& LeftValue) const;

	UFUNCTION(BlueprintCallable, Category = Setup)
	float ReturnLeft();

	UFUNCTION(BlueprintCallable, Category = Setup)
	float ReturnRight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	
	float Left = 0.0;

	float Right = 0.0;

	bool CmdLFwd = false;
	bool CmdLRev = false;
	bool CmdRFwd = false;
	bool CmdRRev = false;

	UTankTrack* RightTrack = nullptr;
	UTankTrack* LeftTrack = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float IncrementPerSecond = 0.05;

	float MovementIncrement();
};
