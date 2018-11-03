// Fill out your copyright notice in the Description page of Project Settings.

#include "RealTankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ARealTankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

// Called every frame
void ARealTankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ARealTankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn); // need to call super when overriding

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank))
		{
			return;
		}

		// Subscribe our local method to the tanks death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ARealTankPlayerController::OnPossedTankDeath);
	}
}

void ARealTankPlayerController::OnPossedTankDeath()
{
	StartSpectatingOnly();
}

void ARealTankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()){ return; } // e.g. if not possessing
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // Out parameter
	if (GetSightRayHitLocation(HitLocation)) // Has side effect, going to line trace
	{
		AimingComponent->AimAt(HitLocation);
	}
}

// Get world location through a linetrace from crosshair, true if it hits landscape
bool ARealTankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	//OutHitLocation = FVector(1.0); // Going to return a vector that gives 1,1,1 because it is a pointer - The variable can change because it is a reference pointer.

	// Find crosshair position in pixel coordinates
	int32 ViewPortSizeX, ViewPortSizeY;
	GetViewportSize(ViewPortSizeX, ViewPortSizeY); /// Finds game window size

	auto ScreenLocation = FVector2D(ViewPortSizeX * CrossHairXLocation, ViewPortSizeY * CrossHairYLocation); 

	// "De-project" the screen position of the crosshair to a world direction
	FVector LookDirection; /// value is being set by the GetLookDirection method since its using the "FVector& LookDirection"
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// Line-trace through that look direction, and see what we hit (up to a maximum range)
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}
	
	return false;
}

bool ARealTankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; /// this is to be ignored - though we need to pass it into the DeprojectScreenPositionToWorld() so it needs to be here.

	DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
	return true;
}

bool ARealTankPlayerController::GetLookVectorHitLocation(FVector& LookDirection, FVector& OutHitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	FCollisionQueryParams TraceCollisionParameters = FCollisionQueryParams();
	TraceCollisionParameters.AddIgnoredActor(GetPawn());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, TraceCollisionParameters))
	{
		OutHitLocation = HitResult.Location;
		return true;
	}
	OutHitLocation = FVector(0.0f);
	return false;
}

