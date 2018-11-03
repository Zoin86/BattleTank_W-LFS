// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Projectile.h"
#include "TankBarrel.h"/// Has to be included because of the forward declaration - for using the methods below in this file
#include "TankTurret.h"/// Furthermore the CCP files name associated member header file has to be the first included header file

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; //TODO Should this really tick?

	// ...
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	AmmoCount = MaxAmmo;
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AmmoCount <= 0)
	{
		FiringState = EFiringState::Empty;
	}
	else if((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Turret)){return;}
	if (!ensure(Barrel)){return;}

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName ("Projectile")); /// Barrel is a static mesh component so "Components/StaticMeshComponent.h" needs to be included

	/// UGameplayStatics because its a 'static' method - Check unreal docs to see whether a method might be static or not
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0.0f, 0.0f, ESuggestProjVelocityTraceOption::DoNotTrace, FCollisionResponseParams::DefaultResponseParam,TArray<AActor*>(), false);
		///DoNotTrace needs to be there to prevent bug |----- Just using this to get to the debugger -----------------|
	
	if (bHaveAimSolution) 	
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		RotateTurretTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret))	{ return; }

	// Work out difference between current barrel rotation, and AimDirection
	auto BarrelRotation = Barrel->GetComponentRotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotation; /// Delta is the distance in rotation between its current rotation to its target rotation

	Barrel->Elevate(DeltaRotator.Pitch); /// 1 equals that of 100 of the maximum - -1 would mean -100 so it would move the opposite direction
}

void UTankAimingComponent::RotateTurretTowards(FVector AimDirection)
{
	if (!ensure(Turret)) { return; }

	auto TurretRotation = Turret->GetComponentRotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - TurretRotation;

	// Always rotate the shortest way
	if (FMath::Abs(FMath::Abs(DeltaRotator.Yaw)) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else // Avoid going the long-way round
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}

}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)){return false;}

	if (Barrel->GetForwardVector().Equals(AimDirection, AimLockStateTolerance))
	{
		return false;
	}
	else
	{
		return true;
	}

}

void UTankAimingComponent::Fire()
{
	if(FiringState != EFiringState::Reloading && AmmoCount > 0)
	{
		if (!ensure(ProjectileBlueprint)){ return; }
		if (!ensure(Barrel)){ return; }
		// spawn a projectile at the projectile socket of the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketTransform(FName("Projectile")));

		Projectile->LaunchProjectile(LaunchSpeed);
		AmmoCount --;
		LastFireTime = FPlatformTime::Seconds();
	}
}