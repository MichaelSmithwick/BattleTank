// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// enumerations require special handling to be UE4 compatible
UENUM()
enum class EFiringStatus : uint8
{
	Reloading,
	Aiming,
	Locked
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	bool Reloading();

	// called to handle aiming for the tank
	void AimAt(FVector HitLocation);

	// Move tank barrel
	bool MoveBarrelTowards(FVector AimDirection);

	// Move tank turret
	bool MoveTurretTowards(FVector AimDirection);

	// fire the projectile
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Fire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Firing Status
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Locked;

	// Tank barrel and turret are captured
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	// The firing solution when the turret and barrel are within
	// plus/minus this value on either side of 0.0
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LockError = 0.5;

	// The time delay before next shot is ready
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReloadTimeInSeconds = 3.0;

	// The speed in cm/s of the launched projectile
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LaunchSpeed = 5000;

	// The blueprint of the projectile (must be AProjectile class)
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

private:
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	bool bTargetLock = false;

	double LastFireTime = 0;

	bool AimAndLock(const FVector & OutLaunchVelocity);

};
