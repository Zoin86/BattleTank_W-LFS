// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming state
UENUM()
enum class EFiringState : uint8
{
	Locked,
	Aiming,
	Reloading,
	Empty
};

/// This is a forward Declaration
class UTankBarrel; 
class UTankTurret;
class AProjectile;

// Holds Barrel properties
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANKS_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Projectile)
	void Fire();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	void AimAt(FVector HitLocation);

	EFiringState GetFiringState() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int32 MaxAmmo = 15;

	UPROPERTY(BlueprintReadOnly, Category = Projectile)
	int32 AmmoCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringState FiringState = EFiringState::Reloading;

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	UPROPERTY(EditAnywhere, Category = "Firing")
	float LaunchSpeed = 4000.0f;

	UPROPERTY(EditAnywhere, Category = Setup) /// You could set EditDefault which would only allow changes to this value in the blueprint file
	float ReloadTimeInSeconds = 3.0f;

	FVector AimDirection;

	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<AProjectile>ProjectileBlueprint;

	UPROPERTY(EditAnywhere, Category = Setup)
	float AimLockStateTolerance = 0.03;

	double LastFireTime = 0;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	void MoveBarrelTowards(FVector AimDirection);
	void RotateTurretTowards(FVector AimDirection);
	
	bool IsBarrelMoving();
	
};
