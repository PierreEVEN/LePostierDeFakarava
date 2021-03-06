// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "CoreMinimal.h"
#include "Vehicle/TargetableActor.h"
#include "PlaneBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class LEPOSTIERDEFAKARAVA_API APlaneBase : public ATargetableActor
{
	GENERATED_BODY()

public:

	APlaneBase(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RootPhysicMesh = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Aerodynamics")
	void GetDragFromDirection(const FVector& VelocityVector, float& outDragSurface, FVector& outDragPoint, float& outDragForce, int32 Precision = 20, float AirDensity = 0.001f, float DragCoefficient = 0.3f, bool bUseComplexeCollisions = true, bool bDrawDebugTraces = false);


};
