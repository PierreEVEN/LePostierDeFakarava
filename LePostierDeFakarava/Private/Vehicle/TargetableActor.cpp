// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../public/Vehicle/TargetableActor.h"

// Sets default values
ATargetableActor::ATargetableActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATargetableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATargetableActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

