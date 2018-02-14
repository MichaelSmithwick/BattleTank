// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Public/TimerManager.h"
#include "Engine/World.h"

int32 AProjectile::counter = 0;

// This object is created when a projectile is fired and destroy after a short time delay when it hits something
// The root of the projectile is an object mesh assigned through the blueprint (a round ball in this case)
// The object mesh has assigned to it: (1) Initial launch particle system; (2) Hit particle system; (3) a Radial Force.
// The object mesh has a UProjectileMovementComponent associated with it 
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// create program shell to contain and control the projectile static mesh and set as the root for projectile object
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);

	// notify the mesh on collision. turn off visiblity in normal use
	CollisionMesh->SetNotifyRigidBodyCollision(true);  // "Simulation generates hit" blueprint property
	CollisionMesh->SetVisibility(false); // will set to true (in blueprint) for testing to enable projectile visibility 

	// LauchBlast is the program shell for the blast particle called on projectile launch
	// this particle is set to autoactivate when this object becomes live in the game
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// ImpactBlast is the program shell for the blast particle called when the projectile hits something
	// turn it off for now -- activate it when the projectile hits something
	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false; // will not be activated automatically on creation
	ImpactBlast->SetVisibility(true); // must be visible to be seen!

	// Create force object to be used when this projectile hits something
	ExplosiveForce = CreateDefaultSubobject<URadialForceComponent>(FName("Radial Force"));
	ExplosiveForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosiveForce->bAutoActivate = false; // will not be activated automatically on creation

	// Create the projectile movement, activated when this projectile is launched
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false; // will not be activated automatically on creation

	ThisProjectilesNumber = AProjectile::counter++;
}

// Sets up OnHit() callback to handle when object hits something
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// add callback function to handle when the projectile hits an object
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called by the TankAimingComponent to move the projectile through the air
void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}

// When the projectile hits something the blast & smoke traveling with the projectile will
// be deactivated and the impact explosion will be activated.
// A time delay callback function will be set to destroy this object after a short time delay.
void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	// deactivate launch visual, activate impact visual, trigger force effect
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosiveForce->FireImpulse();

	// It is necessary to set the root component to another item (why?) so this UE component
	// can be destroyed. Note this is not the C++ object being destroyed here.
	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	// Set timer callback function to destroy this C++ object.
	// The short time delay provides opportunity for the ImpactBlast smoke to display prior to destruction.
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectile::KillTimer, DestroyTimeDelay,false);
}

void AProjectile::KillTimer()
{
	this->Destroy();
}
