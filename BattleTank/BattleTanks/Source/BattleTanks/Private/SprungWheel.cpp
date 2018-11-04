// Copyright Simon Sejer Nielsen

#include "SprungWheel.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	SetupConstraint();

}

void ASprungWheel::SetupConstraint()
{
	if (!GetAttachParentActor()){return;}

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!BodyRoot){	return; }
	MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
	AxleWheelConstrain->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);

	Axle->SetMassOverrideInKg(NAME_None, BodyRoot->GetMass() / 40);
	Wheel->SetMassOverrideInKg(NAME_None, Axle->GetMass() / 4);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

