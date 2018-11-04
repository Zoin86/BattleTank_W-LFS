// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "SprungWheel.h"


UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	return TArray<ASprungWheel*>();
}

void UTankTrack::SetThrottle(float Throttle)
{
	float Throttle = FMath::Clamp<float>(Throttle, -1, 1);
	DriveTrack(Throttle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied =  CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWHeel = ForceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWHeel);
	}
}
