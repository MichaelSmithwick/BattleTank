// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "TankAimingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// called to initialize object before beginning play
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	FiringStatus = EFiringStatus::Reloading;

	// make tank load up ammo before first fire
	LastFireTime = FPlatformTime::Seconds();

	// load up the ammo!
	CurrentAmmo = InitialAmmoSupply;

}

// called every Tick
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (CurrentAmmo <= 0)
	{
		FiringStatus = EFiringStatus::Empty;
	}
	else if (Reloading())
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}
}

// returns true if in reloading cycle, otherwise false is returned
bool UTankAimingComponent::Reloading()
{
	return (FPlatformTime::Seconds() - LastFireTime) <= ReloadTimeInSeconds;
}

// If barrel forward vector does not equal vector to aim point
// then the barrel will be moving.
bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel))
	{
		return false;
	}

	FVector BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.01);
}

// moves barrel and turret towards target firing solution and possibly locks on firing solution
// returns true if target is locked, false if it is not
bool UTankAimingComponent::AimAndLock(const FVector& OutLaunchVelocity)
{
	bTargetLock = false;

	AimDirection = OutLaunchVelocity.GetSafeNormal();

	// move barrel and turret without regard to reloading status
	MoveBarrel();
	MoveTurret();

	// if reloading then target lock and aiming is not possible
	if (!Reloading())
	{
		if (!IsBarrelMoving())
		{
			bTargetLock = true;
		}
	}

	return bTargetLock;
}

// calculate aiming angle from this tank to HitLocation
void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if(ensure(Barrel && Turret))
	{
		FVector OutLaunchVelocity = FVector(0);
		FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile")); // currently at business end of barrel

		bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
			this,
			OutLaunchVelocity, // initial velocity is 0.0
			StartLocation,     // end of barrel where Projectile socket is
			HitLocation,       // from TankController (Player or AI)
			LaunchSpeed,       // blueprint settable option
			false,
			0.0,
			0.0,
			ESuggestProjVelocityTraceOption::DoNotTrace
		);

		// if there is a target (firing solution) then move barrel and turret towards it.
		if (bHaveAimSolution)
		{
			AimAndLock(OutLaunchVelocity);
		}
	}
}

// pitch barrel up or down to get correct elevation for firing at AimDirection
// return true if the elevation is at correct elevation within error margin LockError
bool UTankAimingComponent::MoveBarrel()
{
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->ElevateBarrel(DeltaRotator.Pitch);

	// TODO Change LockError to be percentage instead of a fixed value
	return DeltaRotator.Pitch < LockError;
}

// rotate turret from current position towards target at AimDirection
// return true if turret is aligned with target within tolerance of LockError
bool UTankAimingComponent::MoveTurret()
{
	FRotator TurretRotator = Turret->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - TurretRotator;

	if (FMath::Abs(DeltaRotator.Yaw) > 180.0)
	{
		DeltaRotator = TurretRotator - AimAsRotator;
	}

	Turret->RotateTurret(DeltaRotator.Yaw);

	return DeltaRotator.Yaw < LockError;
}

// Capture pointers to instantiated barrel and turret from blueprint
void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// If the firing solution is locked then fire the projectile
void UTankAimingComponent::Fire()
{
	// FString Name = GetOwner()->GetName();
	// UE_LOG(LogTemp,Warning,TEXT("ALBATROSS -- %s -> CurrentAmmo = %d"), *Name, CurrentAmmo)

	// no barrel or no projectile then no firing
	if (!ensure(Barrel && ProjectileBlueprint))
	{
		return;
	}

	// if there is a firing solution and we are not reloading then fire and we have ammo
	if(!Reloading() && !IsBarrelMoving() && (CurrentAmmo > 0))
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);

		CurrentAmmo--;

		LastFireTime = FPlatformTime::Seconds();  // capture the fireing time to calc reloading time
	}
}

// return status of firing state: reloading, aiming, locked, or empty
EFiringStatus UTankAimingComponent::GetFiringStatus() const
{
	return FiringStatus;
}

// return ammo quantity to caller
int32 UTankAimingComponent::GetAmmoLeft() const
{
	return CurrentAmmo;
}

