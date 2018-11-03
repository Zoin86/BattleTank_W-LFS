// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"
// AI controller has a dependency on movement component via pathfinding ( MoveToActor() )


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn); // need to call super when overriding
	
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (PossessedTank){return;}

		// Subscribe our local method to the tanks death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossedTankDeath);
	}
}

void ATankAIController::OnPossedTankDeath()
{
	if (!ensure(GetPawn())){return;} // TODO - Remove if okay

	GetPawn()->DetachFromControllerPendingDestroy();
	UE_LOG(LogTemp, Warning, TEXT("Tank OnDeath Event Recieved"))
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn(); /// Casting this we make sure that it finds the player tank, and not just any player pawn
	auto ControlledTank = GetPawn();

	if (!PlayerTank && ControlledTank){ return; }
	
	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);
	
	// Aim at the player
	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	
	// fire if aim or locked
	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire(); // TODO Limit firing rate 
	}
	

}

