
#include "Landscape/ProceduralLandscapeSection_Implementation.h"
#include "Landscape/ProceduralLandscapeOctreeNode_Implementation.h"
#include "ProceduralMeshComponent.h"
#include "Engine/Engine.h"

AProceduralLandscapeWrapper* mainLandscape;

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
	mainLandscape = this;
	rootLandscapeSection = new ProceduralLandscapeSectionImplementation(this, FProceduralLandscapeVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
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

int32 AProceduralLandscapeWrapper::AddMeshSection(const FUnrealFormatedSectionData& sectionData)
{
	int32 sectionIndex;

	if (FreeSections.Num() > 0)
	{
		sectionIndex = FreeSections[FreeSections.Num() - 1];
		FreeSections.RemoveAt(FreeSections.Num() - 1);
	}
	else
	{
		sectionIndex = LandscapeMesh->GetNumSections();
	}

	LandscapeMesh->CreateMeshSection(sectionIndex, sectionData.Vertices, sectionData.Indices, sectionData.Normals, sectionData.UVs, sectionData.VertexColors, sectionData.Tangents, bGenerateCollisions);
	LandscapeMesh->SetMaterial(sectionIndex, LandscapeMaterial);
	return sectionIndex;
}

void AProceduralLandscapeWrapper::RemoveMeshSection(int32 sectionIndice)
{
	LandscapeMesh->ClearMeshSection(sectionIndice);
	FreeSections.Add(sectionIndice);
}

AProceduralLandscapeWrapper* AProceduralLandscapeWrapper::GetLandscape()
{
	return mainLandscape;
}

bool AProceduralLandscapeWrapper::IsLocationUnderLandscape(FVector position)
{
	if (GetLandscape())
	{
		if (GetLandscape()->GetAltitudeAtLocation(position.X, position.Y) < position.Z)
		{
			return true;
		}
	}
	return false;
}

ProceduralLandscapeSectionImplementation::ProceduralLandscapeSectionImplementation(AProceduralLandscapeWrapper* wrapper, FProceduralLandscapeVector sectionPosition)
	: ProceduralLandscapeSection(sectionPosition)
{
	parentWrapper = wrapper;
	parentOctreeNode = GenerateOctreeNode(ProceduralLandscapeVector2D(sectionPosition.X, sectionPosition.Y), parentWrapper->baseSectionHalfSize, 0);
}

std::vector<FProceduralFoliageType> ProceduralLandscapeSectionImplementation::GetFoliageTypes() const
{
	std::vector<FProceduralFoliageType> outFoliages;
	for (const auto& foliage : parentWrapper->foliages)
	{
		outFoliages.push_back(foliage.ToFoliageType());
	}
	return outFoliages;
}
