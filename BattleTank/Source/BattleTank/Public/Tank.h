// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// Called by the engine when actor damage is needed
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Return current health as a percentage of starting health, between 0 and 1
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	FTankDelegate OnDeath;


	// -------------- My Functions ------------------------

	// returns Tanks health on scale 0=dead to 100=prime health
	UFUNCTION(BlueprintCallable, Category = "Health")
	int32 GetCurrentHealth();

	// Tank alive = true, false otherwise
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive();

	// Tank alive = false, true otherwise
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsNotAlive();

	// Returns true if this object does not move
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsGunOnly();

	float UE4Sample_TakeDamage(float &Damage, const FDamageEvent & DamageEvent, AController * EventInstigator, AActor * DamageCauser);

protected:
	virtual void BeginPlay() override;

private:
	float FloatCurrentHealth;  // used by UE4Sample_TakeDamage()

	// Health of the Tank
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth = 0;

	// Prevents Path Finding if true, allows movement if false
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool GunOnly = false;
};
