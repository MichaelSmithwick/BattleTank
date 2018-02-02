// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	// barrel setter
	void SetBarrelReference(UTankBarrel* BarrelToSet);
	void SetTurretReference(UStaticMeshComponent* TurretToSet);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// called to handle aiming for the tank
	void AimAt(FVector HitLocation, float LaunchSpeed);

	// Move tank barrel
	void MoveBarrelTowards(FVector AimDirection);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Firing)
	FVector BarrelLocation = FVector(0);

private:
	UTankBarrel* Barrel = nullptr;
	UStaticMeshComponent* Turret = nullptr;
	
};
