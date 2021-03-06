// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TargetableActor.generated.h"

UCLASS()
class LEPOSTIERDEFAKARAVA_API ATargetableActor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATargetableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
