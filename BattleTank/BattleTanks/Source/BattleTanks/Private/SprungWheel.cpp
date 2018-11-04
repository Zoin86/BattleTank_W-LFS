// Copyright Simon Sejer Nielsen

#include "SprungWheel.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
	SetRootComponent(MassWheelConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(MassWheelConstraint);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);

	AxleWheelConstrain = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstrain"));
	AxleWheelConstrain->SetupAttachment(Axle);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);
	SetupConstraint();

}

void ASprungWheel::SetupConstraint()
{
	if (!GetAttachParentActor()){return;}

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!BodyRoot){	return; }
	MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
	AxleWheelConstrain->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);

	// TODO - Find solution to fix the tank tilting
	// Automatically add mass to wheel and Axel depending on the mass from its parent
	//Axle->SetMassOverrideInKg(NAME_None, BodyRoot->GetMass() / 140);
	//Wheel->SetMassOverrideInKg(NAME_None, Axle->GetMass() / 14);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		// Need to reset for ready it up for the next frame
		TotalForceMagnitudeThisFrame = 0;
	}	
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
	// Wheel->AddForce(Axle->GetForwardVector() * ForceMagnitude);
}

void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}

void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ApplyForce();
}

