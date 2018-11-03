// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TankTrack.generated.h"

/**
 * Tank Track used to set maximum driving force, and apply it to the tank
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANKS_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

private:
	UTankTrack();

	UFUNCTION() // UFUNCTION required for this because of "Unreal Magic"
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	float CurrentThrottle = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void ApplySideWaysForce();

	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

	void DriveTrack();
	
	// Max for per track in newtons
	UPROPERTY(EditAnywhere, Category = Setup)
	float TrackMaxDrivingForce = 40000000.f; // 40ton Tank at 10m/s^2 - also 40 million equals that of 400.000 newtons
};
