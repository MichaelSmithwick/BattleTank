// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrel;
class UTankTurret;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	// barrel setter
	void SetBarrelReference(UTankBarrel* BarrelToSet);
	void SetTurretReference(UTankTurret* TurretToSet);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// called to handle aiming for the tank
	void AimAt(FVector HitLocation, float LaunchSpeed);

	// Move tank barrel
	bool MoveBarrelTowards(FVector AimDirection);

	// Move tank turret
	bool MoveTurretTowards(FVector AimDirection);

	// Is the target in the sights? (true if it is, false otherwise)
	bool TargetLocked();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// TODO Remove BarrelLocation & TurretLocation if not needed
	// UPROPERTY(EditAnywhere, Category = Firing)
	// FVector BarrelLocation = FVector(0);

	// UPROPERTY(EditAnywhere, Category = Firing)
	// FVector TurretLocation = FVector(0);

private:
	void ClearTargetLock();

	void SetTargetLock();

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LockError = 0.5;

	bool TargetLock = false;
	
};
