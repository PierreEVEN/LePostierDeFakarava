#include "../../Public/Landscape/ProceduralLandscapeOctreeNode_Implementation.h"
#include "Landscape/ProceduralLandscapeSection_Implementation.h"
#include "Engine/Engine.h"

FProceduralLandscapeOctreeNodeImplementation::FProceduralLandscapeOctreeNodeImplementation(FProceduralLandscapeSectionImplementation* parent, const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel)
	: ProceduralLandscapeOctreeNode(nodeLocation, halfSize, lodLevel) {
	parentSection = parent;

	bIsMeshDataGenerated = false;
	currentNodeStatus = NodeGenerationStatus::NodeGenerationStatus_None;
	GEngine->AddOnScreenDebugMessage(rand(), 5.f, FColor::Red, "Starting mesh generation...");
	RequestMeshGeneration();
}

unsigned int FProceduralLandscapeOctreeNodeImplementation::GetNodeMeshResolution() const
{
	return parentSection->parentWrapper->NodeMeshResolution;
}


bool FProceduralLandscapeOctreeNodeImplementation::EnableAsyncMeshGeneration() const
{
	return parentSection->parentWrapper->bEnableAsyncMeshGeneration;
}

void FProceduralLandscapeOctreeNodeImplementation::GetInformationsAtLocation(float posX, float posY, ProceduralLandscapeLocationData& data)
{
	data.altitude = parentSection->parentWrapper->GetAltitudeAtLocation(posX, posY);
}

void FProceduralLandscapeOctreeNodeImplementation::PostVerticesCreation()
{
	unrealFormatedData = FUnrealFormatedSectionData(NodeMeshData);
}

void FProceduralLandscapeOctreeNodeImplementation::OnMeshGenerated()
{
	GEngine->AddOnScreenDebugMessage(rand(), 5.f, FColor::Red, "Mesh successfully generated");
	parentSection->parentWrapper->AddMeshSection(unrealFormatedData);
}
