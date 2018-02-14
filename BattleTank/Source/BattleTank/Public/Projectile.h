// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Implements UProjectileMovementComponent to move projectile to target
	void LaunchProjectile(float Speed);

private:
	// disables initial particle display, enables hit particle display and sets timer for object destruction
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// destroys this projectile object after a short time delay
	void KillTimer();

	// TODO Determine how to make launch smoke dissipate before erasing it - aesthetically bad as it is now

	// delay in seconds after hit detected before this object is destroyed
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyTimeDelay = 30.0; // seconds

	// Moves projectile from source to target along predetermined path
	UProjectileMovementComponent * ProjectileMovement = nullptr;

	// projectile mesh is set in blueprint
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* CollisionMesh = nullptr;

	// initial blast particle component is set in blueprint
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* LaunchBlast = nullptr;

	// hit blast particle component is set in blueprint
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ImpactBlast = nullptr;

	// a force that expands outward from a center point
	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* ExplosiveForce = nullptr;

	static int32 counter; // counts how many projectiles have been created -- warning, may overflow!!

	int32 ThisProjectilesNumber; // stores the counter variable at the time this projectile was created
};
