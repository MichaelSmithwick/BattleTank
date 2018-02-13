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

	void LaunchProjectile(float Speed);

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void KillTimer();

	// delay in seconds after hit detected before this object is destroyed
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyTimeDelay = 6.0; // seconds

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
