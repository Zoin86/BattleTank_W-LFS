// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit); // This is a tricky thing - Needs to be here for OnComponentHit() To work!
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySideWaysForce();
	CurrentThrottle = 0;
}


void UTankTrack::ApplySideWaysForce()
{									// If the tank is moving along just the x axis at a speed of 40 cm/s, and it is facing 60 degrees to the right of the x-axis, what will the SlippageSpeed be?
	// Get Rightvector Velocity		// DotProduct Math Example: The right vector is 90 degrees to the right of the tank heading making it 60+90 = 150 degrees right of the x-axis. So cos(150) * 40 = -35 (Rounded)
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	// Calculate the required acceleration this frame to correct
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	// Get Root Component
	// Instead of Casting it every tick, could cache it as a variable in the header file
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	// Calculate and apply sideways force (F = m * a )
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

void UTankTrack::DriveTrack()
{
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}
