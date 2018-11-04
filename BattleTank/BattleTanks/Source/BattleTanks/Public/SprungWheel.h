// Copyright Simon Sejer Nielsen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "SprungWheel.generated.h"

UCLASS()
class BATTLETANKS_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprungWheel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupConstraint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddDrivingForce(float ForceMagnitude);

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = Component)
	USphereComponent* Wheel = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Component)
	USphereComponent* Axle = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Component)
	UPhysicsConstraintComponent* MassWheelConstraint = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Component)
	UPhysicsConstraintComponent* AxleWheelConstrain = nullptr;

	float TotalForceMagnitudeThisFrame = 0.f;
	void ApplyForce();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
