// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleTanks.h"
#include "Public/UObject/Class.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "RealTankPlayerController.generated.h" // Generated files must be the last include

/**
 * 
 */

 /// Forward Declaration
class UTankAimingComponent;
class ATank;

UCLASS()
class BATTLETANKS_API ARealTankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Start the tank moving the barrel so that a shot would hit where the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return Out Paremeter, true if it hits the landscape
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
	void FoundAimingComponent(UTankAimingComponent* AimingCompRef);

private:
	virtual void Tick(float DeltaTime) override;

	void SetPawn(APawn * InPawn);

	UTankAimingComponent* AimingComponent = nullptr;

	// Needs to be a UFUNCTION when wanting to subscribe to delegate events
	UFUNCTION()
	void OnPossedTankDeath();

	UPROPERTY(EditAnywhere)
	float CrossHairXLocation = 0.5f;
	UPROPERTY(EditAnywhere)
	float CrossHairYLocation = 0.33333f;
	UPROPERTY(EditAnywhere, Category = "Look Range")
	float LineTraceRange = 100000.0f;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(FVector& LookDirection,  FVector& HitLocation) const;
};
