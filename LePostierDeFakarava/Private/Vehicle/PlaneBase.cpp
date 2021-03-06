// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../Public/Vehicle/PlaneBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APlaneBase::APlaneBase(const class FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	RootPhysicMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RootPhysicMesh"));
	SetRootComponent(RootPhysicMesh);
	RootPhysicMesh->SetSimulatePhysics(true);
}

void APlaneBase::GetDragFromDirection(const FVector& VelocityVector, float& OutDragSurface, FVector& OutDragPoint, float& OutDragForce, int32 Precision /*= 20*/, float AirDensity /*= 0.001*/, float DragCoefficient /*= 0.3*/, bool bUseComplexeCollisions /*= true*/, bool bDrawDebugTraces /*= false*/)
{
	if (!RootPhysicMesh) return;

	FVector VelocityForwardVector = VelocityVector;
	VelocityForwardVector.Normalize();
	FVector VelocityRightVector = FVector::CrossProduct(VelocityForwardVector, GetActorUpVector());
	FVector VelocityUpVector = FVector::CrossProduct(VelocityForwardVector, VelocityRightVector);

	FVector BoundOrigin = RootPhysicMesh->Bounds.Origin;
	FVector CenterOfMass = RootPhysicMesh->GetCenterOfMass();
	float TestRadius = RootPhysicMesh->Bounds.SphereRadius;
	float UnitScale = (TestRadius / 100.f) / Precision;
	UnitScale *= UnitScale;

	OutDragPoint = CenterOfMass;
	OutDragSurface = 0.f;

	FCollisionQueryParams traceParams;
	traceParams.bTraceComplex = bUseComplexeCollisions;

	for (int32 x = -Precision; x < Precision; ++x)
	{
		FVector PosX = VelocityRightVector * ((TestRadius / Precision) * x);

		for (int32 y = -Precision; y < Precision; ++y)
		{
			FVector PosY = VelocityUpVector * ((TestRadius / Precision) * y);
			FVector PosZ = VelocityForwardVector * TestRadius;
			

			FVector TraceStart = BoundOrigin + PosX + PosY + PosZ;
			FVector TraceEnd = BoundOrigin + PosX + PosY - PosZ;

			FHitResult hitResult;

			if (RootPhysicMesh->LineTraceComponent(hitResult, TraceStart, TraceEnd, traceParams))
			{
				float ImpactWeight = FVector::DotProduct(hitResult.Normal, VelocityForwardVector);

				OutDragSurface += ImpactWeight * UnitScale;

				FVector CenterToHitDirection = CenterOfMass - hitResult.Location;
				CenterToHitDirection.Normalize();

				OutDragPoint += CenterToHitDirection * ImpactWeight * 0.1 * (TestRadius / Precision);

				if (bDrawDebugTraces)
				{
					UKismetSystemLibrary::DrawDebugLine(GetWorld(), hitResult.ImpactPoint, hitResult.ImpactPoint - VelocityForwardVector * ImpactWeight * 400.f, FColor::Red, 0.f, 0.f);
				}
			}
			else
			{
				if (bDrawDebugTraces)
				{
					UKismetSystemLibrary::DrawDebugLine(GetWorld(), TraceStart, TraceStart + VelocityForwardVector * 100.f, FColor::Blue, 0.f, 0.f);
					UKismetSystemLibrary::DrawDebugLine(GetWorld(), TraceEnd, TraceEnd - VelocityForwardVector * 100.f, FColor::Blue, 0.f, 0.f);
				}
			}
		}
	}

	float SquaredVelocity = VelocityVector.Size();
	SquaredVelocity *= SquaredVelocity;
	OutDragForce = SquaredVelocity * 0.5 * AirDensity * DragCoefficient * OutDragSurface;
}
