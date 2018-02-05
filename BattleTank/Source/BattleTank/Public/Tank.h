// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankAimingComponent;
class UTankBarrel;
class UTankTurret;
class AProjectile;
class UTankTrack;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetBarrelReference(UTankBarrel* BarrelToSet);

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetTurretReference(UTankTurret* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetRightTrackReference(UTankTrack* TrackToSet);

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetLeftTrackReference(UTankTrack* TrackToSet);

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Fire();

	// Location of whatever the tank is aiming at
	void AimAt(FVector HitLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UTankAimingComponent* TankAimingComponent = nullptr;
	
	UTankBarrel* Barrel = nullptr;

private:

	UTankTrack* RightTrack = nullptr;
	UTankTrack* LeftTrack = nullptr;

	double LastFireTime = 0;

	// The time delay before next shot is ready
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float ReloadTimeInSeconds = 3;

	// The speed in cm/s of the launched projectile
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float LaunchSpeed = 5000;

	// The blueprint of the projectile (must be AProjectile class)
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;
};
