// Copyright(c)2018 -- Mike Smithwick -- All Rights Reserved

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"

int32 AProjectile::counter = 0;

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// create program shell to contain and control the projectile static mesh and set as the root for projectile object
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);

	// notify the mesh on collision. turn off visiblity in normal use
	CollisionMesh->SetNotifyRigidBodyCollision(true);  // "Simulation generates hit" blueprint property
	CollisionMesh->SetVisibility(false); // will set to true for testing to enable visibility of the projectile

	// LauchBlast is the program shell for the blast particle called on projectile launch
	// since the projectile is being created now as part of a launch this has to be activated now as part of that launch
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// ImpactBlast is the program shell for the blast particle called when the projectile hits something
	// turn it off for now -- activate it when the projectile hits something
	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false; // will not be activated automatically on creation

	// controles the projectile movement. create it when needed, not now
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false; // will not be activated automatically on creation

	ThisProjectilesNumber = AProjectile::counter++;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// add callback function to handle when the projectile hits an object
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// use the movement object to move projectile through the air
void AProjectile::LaunchProjectile(float Speed)
{
	// UE_LOG(LogTemp,Warning,TEXT("RHINO --  Speed %f"), Speed)
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}

// when the projectile hits something the blast & smoke traveling with the projectile will
// be deactivated and the impact explosion will be activated.
void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	float Time = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("%f  -- %d : Hit! Hit!"), Time ,ThisProjectilesNumber);
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
}

