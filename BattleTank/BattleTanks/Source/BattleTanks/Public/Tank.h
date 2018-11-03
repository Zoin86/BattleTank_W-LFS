// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

/// Forward Declaration
class UTankBarrel;
class AProjectile;
UCLASS()
class BATTLETANKS_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// Creating Delegate Death Event
	FTankDelegate OnDeath;

	UFUNCTION(BlueprintPure, Category = Health)
	float GetHealthPercentage() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float TankStartingHealth = 100;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float TankHealth = 0.f;

	virtual float TakeDamage(float DamageAmount,	struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
