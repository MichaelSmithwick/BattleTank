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

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!Barrel || !Turret)
	{
		return;
	}

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

	if(bHaveAimSolution)
	{
		ClearTargetLock();

		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();

		bool BarrelLock = MoveBarrelTowards(AimDirection);
		bool TurretLock = MoveTurretTowards(AimDirection);

		// many compilers will optimize out the second operation if the first is false
		// the MoveTurretTowards() function should be carried out even if MoveBarrelTowards() returns false
		if (BarrelLock && TurretLock)
		{
			bool bReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

			if (!bReloaded)
			{
				FiringStatus = EFiringStatus::Reloading;
			}
			else
			{
				FiringStatus = EFiringStatus::Locked;
			}
			SetTargetLock();
		}
		else
		{
			bool bReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

			if (!bReloaded)
			{
				FiringStatus = EFiringStatus::Reloading;
			}
			else
			{
				FiringStatus = EFiringStatus::Aiming;
			}
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

void UTankAimingComponent::ClearTargetLock()
{
	TargetLock = false;
}

void UTankAimingComponent::SetTargetLock()
{
	TargetLock = true;
}

bool UTankAimingComponent::TargetLocked()
{
	return TargetLock;
}

void UTankAimingComponent::Fire()
{
	//UE_LOG(LogTemp,Warning,TEXT("Bang!! Bang!!"))

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

	if (Barrel && isReloaded && TargetLocked())
	{
		// TODO Check ProjectileBlueprint for nullptr value
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

