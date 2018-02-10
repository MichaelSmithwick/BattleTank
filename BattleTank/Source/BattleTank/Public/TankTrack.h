// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent)) // , hidecategories = ("Collision"))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UTankTrack();

	// set throttle level for this track
	void SetThrottle(float Throttle);

	/*UFUNCTION(BlueprintCallable, Category="Collision")*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// called to initialize this Actor
	virtual void BeginPlay() override;


private:
	// Tank Throttle
	float CurrentThrottle = 0.0;

	// correct slide slip in the Tank
	void ApplySidewaysForce();


	// Max force per track in Newtons
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 150000000.0; // assume 400000kg and 1G force

	// Use this track to push Tank forward
	void DriveTrack();
};
