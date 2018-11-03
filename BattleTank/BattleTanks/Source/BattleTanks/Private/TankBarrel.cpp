// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"


void UTankBarrel::Elevate(float RelativeSpeed)
{
	// Move Barrel the right amount this frame
	// Given a max elevation speed and a frame target			
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.f, 1.f); /// Has to use the same type
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;///DeltaTimeSeconds (NOT A METHOD!) for making sure its framerate independant
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	auto Elevation = FMath::Clamp(RawNewElevation, MinElevevationDegrees, MaxElevationDegrees); 

	SetRelativeRotation(FRotator(Elevation, 0.f, 0.f)); // has to have 0.f, 0.f) at the end or the barrel will start rolling and yawing as well
	/// could also just do "SetRelativeRotation(FRotator(FMath::Clamp(RawNewElevation, MinElevevationDegrees, MaxElevationDegrees)), 0, 0))"
	

}