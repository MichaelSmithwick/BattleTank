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
	Locked,
	Empty
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

	// reloading if within "@ReloadTimeInSeconds" of last shot
	bool Reloading();

	// moving if not aligned with aimed at target
	bool IsBarrelMoving();

	// called to handle aiming for the tank
	void AimAt(FVector HitLocation);

	// Move tank barrel
	bool MoveBarrel();

	// Move tank turret
	bool MoveTurret();

	// fire the projectile
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Fire();

	// returns firing status used to control color on reticle
	EFiringStatus GetFiringStatus() const;

	// returns the amount of ammo left to fire
	UFUNCTION(BlueprintCallable, Category = "Input")
	int32 GetAmmoLeft() const;

protected:
	// called to initialize this Actor
	virtual void BeginPlay() override;

	// Tank barrel and turret meshes and objects are captured from blueprint (Settable)
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	// Firing Status (Read Only)
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Reloading;

	// The firing solution when the turret and barrel are within
	// plus/minus this value on either side of 0.0 (Settable)
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LockError = 0.5;

	// The time delay before next shot is ready (Settable)
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReloadTimeInSeconds = 6.0;

	// The speed in cm/s of the launched projectile (Settable)
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LaunchSpeed = 5000;

	// Initial number of shots for the Tank (Settable)
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 InitialAmmoSupply = 5;

	// The blueprint of the projectile (must be AProjectile class) (Settable)
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

private:
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	// indicates if target has been locked
	bool bTargetLock = false;

	// Last time the gun was fired (used for reloading time)
	double LastFireTime = 0;

	// Current Aiming Direction
	FVector AimDirection;

	// Ammo Left
	UPROPERTY(VisibleAnywhere, Category = "Ammo")
	int32 CurrentAmmo = 0;

	bool AimAndLock(const FVector& OutLaunchVelocity);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
