#include "../../Public/Landscape/ProceduralLandscapeOctreeNode_Implementation.h"
#include "Landscape/ProceduralLandscapeSection_Implementation.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

unsigned int ProceduralLandscapeOctreeNodeImplementation::GetNodeMeshResolution() const { return GetParentSection()->parentWrapper->NodeMeshResolution; }


unsigned int ProceduralLandscapeOctreeNodeImplementation::GetMaxLODLevel() const
{
	return GetParentSection()->parentWrapper->maxLodCount;
}

float ProceduralLandscapeOctreeNodeImplementation::GetNodeMaxScreenSize() const
{
	return GetParentSection()->parentWrapper->maxSectionScreenSize;
}

bool ProceduralLandscapeOctreeNodeImplementation::EnableAsyncGeneration() const {	return GetParentSection()->parentWrapper->bEnableAsyncMeshGeneration; }
void ProceduralLandscapeOctreeNodeImplementation::GetInformationsAtLocation(float posX, float posY, ProceduralLandscapeLocationData& data) const { data.altitude = GetParentSection()->parentWrapper->GetAltitudeAtLocation(posX, posY); }
FProceduralLandscapeVector ProceduralLandscapeOctreeNodeImplementation::GetCameraLocation() const {
	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetParentSection()->parentWrapper->GetWorld(), 0))
	{
		return FProceduralLandscapeVector(CameraManager->GetCameraLocation().X, CameraManager->GetCameraLocation().Y, CameraManager->GetCameraLocation().Z);
	}
	return FProceduralLandscapeVector(0, 0, 0);
}
float ProceduralLandscapeOctreeNodeImplementation::GetCameraFov() const
{
	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetParentSection()->parentWrapper->GetWorld(), 0))
	{
		return CameraManager->GetFOVAngle();
	}
	return 90.f;
}

void ProceduralLandscapeOctreeNodeImplementation::PostVerticesCreation()
{
	unrealFormatedData = FUnrealFormatedSectionData(NodeMeshData);
}

void ProceduralLandscapeOctreeNodeImplementation::PostFoliageCreation()
{
	std::vector<FProceduralFoliageType> foliages = parentSection->GetFoliageTypes();

	for (int32 foliageIndice = 0; foliageIndice < NodeFoliageData.size(); ++foliageIndice)
	{
		FUnrealFormatedFoliageData newFoliage;
		newFoliage.meshs = GetParentSection()->parentWrapper->foliages[foliageIndice].mesh;
		for (int instanceIndex = 0; instanceIndex < NodeFoliageData[foliageIndice].Positions.size(); ++instanceIndex)
		{
			FProceduralLandscapeVector pos = NodeFoliageData[foliageIndice].Positions[instanceIndex];
			FProceduralLandscapeRotator rot = NodeFoliageData[foliageIndice].Rotations[instanceIndex];
			FProceduralLandscapeVector scale = NodeFoliageData[foliageIndice].Scales[instanceIndex];
			newFoliage.transforms.Add(FTransform(FRotator(rot.Y, rot.Z, rot.X), FVector(pos.X, pos.Y, pos.Z), FVector(scale.X, scale.Y, scale.Z)));
		}
		unrealFormatedFoliages.Add(newFoliage);
	}
}

void ProceduralLandscapeOctreeNodeImplementation::OnMeshGenerated()
{
	if (generatedSectionIndice < 0)
	{
		generatedSectionIndice = GetParentSection()->parentWrapper->AddMeshSection(unrealFormatedData);
	}
}

void ProceduralLandscapeOctreeNodeImplementation::OnFoliageGenerated()
{
	for (const auto& foliage : unrealFormatedFoliages)
	{
		UHierarchicalInstancedStaticMeshComponent* newHism = NewObject<UHierarchicalInstancedStaticMeshComponent>(GetParentSection()->parentWrapper);
		newHism->SetStaticMesh(foliage.meshs);
		newHism->RegisterComponent();
		for (const auto& transform : foliage.transforms)
		{
			newHism->AddInstanceWorldSpace(transform);
			foliageHisms.Add(newHism);
		}
	}
}

void ProceduralLandscapeOctreeNodeImplementation::OnMeshRemoved()
{
	if (generatedSectionIndice >= 0)
	{
		GetParentSection()->parentWrapper->RemoveMeshSection(generatedSectionIndice);
		generatedSectionIndice = -1;
	}
}

void ProceduralLandscapeOctreeNodeImplementation::OnFoliageRemoved()
{
	for (auto& hism : foliageHisms)
	{
		hism->DestroyComponent();
	}
	foliageHisms = {};
}
