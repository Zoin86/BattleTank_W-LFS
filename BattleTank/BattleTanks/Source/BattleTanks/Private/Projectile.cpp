// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh); // Set as the RootComponent
	CollisionMesh->SetVisibility(false);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); // SetupAttachment(USceneComponent) could work as well, atleast for constructors

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement Component"));
	ProjectileMovement->bAutoActivate = true;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force Component"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit); // This is a tricky thing - Needs to be here for OnComponentHit() To work!
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();

	UGameplayStatics::ApplyRadialDamage(this,
		ProjectileDamage,
		GetActorLocation(),
		ExplosionForce->Radius, // For Consistancy - Will bind the Damage Radiu to the Explosion Impulse Radius
		UDamageType::StaticClass(),
		TArray<AActor*>(),  // Damage all actor by leaving the array empty
		GetInstigator(),
		GetInstigatorController()
	);
	ExplosionForce->FireImpulse();
	
	// SetTimer Delegate needs an out - Only needs a type
	FTimerHandle Timer;
	
	// AProjectile::DestroyOnTimeExpire needs to be called as a delegate so the ampasand is required. - It then calls DestroyOnTimeExpire();
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectile::DestroyOnTimeExpire, DestroyDelay, false);
	CollisionMesh->DestroyComponent();
}

void AProjectile::DestroyOnTimeExpire()
{
	Destroy();
}



