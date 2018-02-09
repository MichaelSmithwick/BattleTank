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
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	FiringStatus = EFiringStatus::Aiming;
	// ...
	
}

// returns true if in reloading cycle, otherwise false is returned
bool UTankAimingComponent::Reloading()
{
	return (FPlatformTime::Seconds() - LastFireTime) <= ReloadTimeInSeconds;
}

// moves barrel and turret towards target firing solution and possibly locks on firing solution
// returns true if target is locked, false if it is not
bool UTankAimingComponent::AimAndLock(const FVector& OutLaunchVelocity)
{
	bTargetLock = false;

	FVector AimDirection = OutLaunchVelocity.GetSafeNormal();

	// move barrel and turret without regard to reloading status
	bool BarrelLock = MoveBarrelTowards(AimDirection);
	bool TurretLock = MoveTurretTowards(AimDirection);

	// if reloading then target lock and aiming is not possible
	if (!Reloading())
	{
		if (BarrelLock && TurretLock)
		{
			FiringStatus = EFiringStatus::Locked;
			bTargetLock = true;
		}
		else
		{
			FiringStatus = EFiringStatus::Aiming;
		}
	}
	else
	{
		FiringStatus = EFiringStatus::Reloading;
	}

	return bTargetLock;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if(ensure(Barrel && Turret))
	{
		FVector OutLaunchVelocity = FVector(0);
		FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

		bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
			this,
			OutLaunchVelocity,
			StartLocation,
			HitLocation,
			LaunchSpeed,
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

bool UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->ElevateBarrel(DeltaRotator.Pitch);

	return DeltaRotator.Pitch < LockError;
}

bool UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
{
	FRotator TurretRotator = Turret->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - TurretRotator;

	Turret->RotateTurret(DeltaRotator.Yaw);

	return DeltaRotator.Yaw < LockError;
}

void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel))
	{
		return;
	}

	if(!Reloading() && bTargetLock)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

