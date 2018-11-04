// Copyright Simon Sejer Nielsen

#include "SpawnActorComponent.h"


// Sets default values for this component's properties
USpawnActorComponent::USpawnActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USpawnActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// SpawnActorDeferred() - Have to spawn without calling constructors, which we can do with SpawnActorDeferred - SpawnActorDeferred() will NOT RUN the Construction script of blueprints to give caller an opportunity to set parameters beforehand. (Look at Next comment)
	auto NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform());
	if (!NewActor){	return;	}

	NewActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

	// Required for SpawnActorDeferred() - Caller is Responsible for invoking construction Manually by calling UGameplayStatics::FinishSpawningActor()
	UGameplayStatics::FinishSpawningActor(NewActor, GetComponentTransform());
}


// Called every frame
void USpawnActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

