// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionSection.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEPOSTIERDEFAKARAVA_API UCollisionSection : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCollisionSection();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
