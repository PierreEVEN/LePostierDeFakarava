#include "../../Public/Landscape/ProceduralLandscapeSection_Implementation.h"
#include "Landscape/ProceduralLandscapeOctreeNode_Implementation.h"
#include "ProceduralMeshComponent.h"
#include "Landscape/ProceduralLandscapeModule/Mesh/ProceduralLandscapeOctreeNode.h"
#include "Engine/Engine.h"

AProceduralLandscapeWrapper::AProceduralLandscapeWrapper(const class FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* localRootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
	SetRootComponent(localRootComponent);

	LandscapeMesh = ObjectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("LandscapeMesh"));
	LandscapeMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	LandscapeMesh->SetWorldLocation(FVector(0));

}

void AProceduralLandscapeWrapper::BeginPlay()
{
	Super::BeginPlay();

	rootLandscapeSection = new FProceduralLandscapeSectionImplementation(this);

}

void AProceduralLandscapeWrapper::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (rootLandscapeSection) delete rootLandscapeSection;
}

void AProceduralLandscapeWrapper::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (rootLandscapeSection) rootLandscapeSection->UpdateSection();
}

void AProceduralLandscapeWrapper::AddMeshSection(FUnrealFormatedSectionData& sectionData)
{
	GEngine->AddOnScreenDebugMessage(rand(), 5.f, FColor::Red, "Finally add section : " + FString::FromInt(sectionData.Vertices.Num()) + " vertices / " + FString::FromInt(sectionData.Indices.Num()) + " triangles");
	int32 sectionIndex = LandscapeMesh->GetNumSections();
	LandscapeMesh->CreateMeshSection(sectionIndex, sectionData.Vertices, sectionData.Indices, sectionData.Normals, sectionData.UVs, sectionData.VertexColors, sectionData.Tangents, false);
	LandscapeMesh->SetMaterial(sectionIndex, LandscapeMaterial);
}

FProceduralLandscapeSectionImplementation::FProceduralLandscapeSectionImplementation(AProceduralLandscapeWrapper* wrapper)
	: ProceduralLandscapeSection()
{
	GEngine->AddOnScreenDebugMessage(rand(), 5.f, FColor::Red, "Create root section");
	parentWrapper = wrapper;
	parentOctreeNode = GenerateOctreeNode(ProceduralLandscapeVector2D(0, 0), parentWrapper->baseSectionHalfSize, 0);
}

unsigned int FProceduralLandscapeSectionImplementation::GetMaxLODLevel() const
{
	return parentWrapper->maxLodCount;
}

ProceduralLandscapeOctreeNode* FProceduralLandscapeSectionImplementation::GenerateOctreeNode(const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel)
{
	GEngine->AddOnScreenDebugMessage(rand(), 5.f, FColor::Red, "Create Octree node");
	return new FProceduralLandscapeOctreeNodeImplementation(this, nodeLocation, halfSize, lodLevel);
}
