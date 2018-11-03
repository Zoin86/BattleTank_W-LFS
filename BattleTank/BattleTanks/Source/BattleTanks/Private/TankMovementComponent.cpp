// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack) || !ensure(RightTrack))
	{ return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
	// TODO Prevent extra speed from pressing extra speed buttons.
	// UE_LOG(LogTemp, Warning, TEXT("Intend Move Forward Throw: %f"), Throw)
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!ensure(LeftTrack) || !ensure(RightTrack))
	{	return;	}
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call Super as we're replacing the functionality

	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	
	// This here essentially makes it so that the AI will try to have you aligned with its broadside
	auto DotForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	auto CrossForwardThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	
	IntendMoveForward(DotForwardThrow);
	IntendTurnRight(CrossForwardThrow);
}
