// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpawnActorComponent.h"


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
	TArray<USceneComponent*> TrackChildren;
	TArray<ASprungWheel*> SprungWheels;
	GetChildrenComponents(true, TrackChildren);

	for (auto Child : TrackChildren)
	{
		auto SpawnPointChild = Cast<USpawnActorComponent>(Child);
		
		if (!SpawnPointChild) continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnComponent();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		
		if (!SprungWheel) continue;

		SprungWheels.Add(SprungWheel);
	}

	return SprungWheels;
}

void UTankTrack::SetThrottle(float Throttle)
{
	Throttle = FMath::Clamp<float>(Throttle, -1, 1);
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
