// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "Tank.h"
#include "Engine/World.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	// initialize Health Level
	CurrentHealth = StartingHealth;
}

// inputs damage to the Tank
float ATank::TakeDamage(float Damage, const FDamageEvent & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(Damage);
	int32 DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, StartingHealth);

	CurrentHealth -= DamageToApply;

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;

		// All functions registered with this instantiations OnDeath.AddUniqueDynamic() function
		// will be called with this broadcast.  The called functions are free to do what they
		// want. The order the functions are called in is indeterminate.
		OnDeath.Broadcast();
	}

	/*
	FString Name = GetName();
	UE_LOG(LogTemp,Warning,TEXT("WALRUS - %s - Damage: %f  DamageToApply: %d Remaining Health: %d - Status is %s"),
		*Name,
		Damage,
		DamageToApply,
		CurrentHealth,
		*(FString((CurrentHealth!=0)?"Alive":"Dead"))
	)
	*/

	return (float)DamageToApply;
}

// Return current health level as a percentage of the starting health, range 0.0 to 1.0
float ATank::GetHealthPercent() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}


// This is the UE4 way of handling damage according to their documentation. Udemy method is a little different as shown above.
float ATank::UE4Sample_TakeDamage(float &Damage, const FDamageEvent & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// clamp damage to the CurrentHealth range -- why?
	float InitialDamage = Damage;
	Damage = FMath::Clamp<float>(Damage, 0.0, FloatCurrentHealth);

	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		FloatCurrentHealth -= ActualDamage;
		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (FloatCurrentHealth <= 0.f)
		{
			SetLifeSpan(0.001f);
			FloatCurrentHealth = 0.0f;
		}
	}

	return ActualDamage;
}

// returns Tank health on scale 0 to 100 where 0==dead, 100==prime condition
int32 ATank::GetCurrentHealth()
{
	return CurrentHealth;
}

// @returns true if Tank is alive, false if not
bool ATank::IsAlive()
{
	return GetCurrentHealth()>0?true:false;
}

bool ATank::IsNotAlive()
{
	return GetCurrentHealth()>0 ? false : true;
}

// returns true if this tank has no movement capability, false otherwise
bool ATank::IsGunOnly()
{
	return GunOnly;
}
