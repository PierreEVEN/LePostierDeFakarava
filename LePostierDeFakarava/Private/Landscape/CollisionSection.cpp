// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../Public/Landscape/CollisionSection.h"

// Sets default values for this component's properties
UCollisionSection::UCollisionSection()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCollisionSection::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UCollisionSection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

