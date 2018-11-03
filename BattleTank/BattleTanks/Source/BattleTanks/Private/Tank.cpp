// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankHealth = TankStartingHealth;
}

float ATank::GetHealthPercentage() const
{
	return TankHealth / TankStartingHealth;
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = FMath::Clamp<float>(DamageAmount, 0.0f, TankHealth);
	TankHealth -= DamageToApply;
	if (TankHealth <= 0)
	{
		OnDeath.Broadcast();
	}
	return DamageToApply;
}