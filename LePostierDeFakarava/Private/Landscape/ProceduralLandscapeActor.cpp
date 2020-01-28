// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../Public/Landscape/ProceduralLandscapeActor.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SimplexNoiseBPLibrary.h"

DECLARE_STATS_GROUP(TEXT("Landscape stats"), STATGROUP_ProcLandscape, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Terrain Generation"), STAT_TerrainGeneration, STATGROUP_ProcLandscape);
DECLARE_CYCLE_STAT(TEXT("Terrain update"), STAT_TerrainUpdate, STATGROUP_ProcLandscape);
DECLARE_CYCLE_STAT(TEXT("Terrain info update"), STAT_TerrainInfoUpdate, STATGROUP_ProcLandscape);


// Sets default values
AProceduralLandscapeActor::AProceduralLandscapeActor(const class FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootTestComponent = ObjectInitializer.CreateDefaultSubobject<UProceduralLandscapeComponent>(this, TEXT("rootTestComponent"));
	SetRootComponent(rootTestComponent);
	rootTestComponent->AttachMeshComponent(ObjectInitializer);
}

// Called when the game starts or when spawned
void AProceduralLandscapeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralLandscapeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





void UProceduralLandscapeComponent::AttachMeshComponent(const class FObjectInitializer& ObjectInitializer)
{
	LandscapeMesh = ObjectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("LandscapeMesh"));
	LandscapeMesh->SetWorldLocation(FVector(0));
}

UProceduralLandscapeComponent::UProceduralLandscapeComponent(const class FObjectInitializer& ObjectInitializer)
{
	OctreeSectionClass = UProcLandscapeOctreeElem::StaticClass();
}

void UProceduralLandscapeComponent::BeginPlay()
{
	Super::BeginPlay();
	RootSection = NewObject<UProcLandscapeOctreeElem>(this, OctreeSectionClass);
	RootSection->Initialize(this, GetComponentLocation(), BaseSectionHalfSize, BaseSectionHalfSize, 0);
}

void UProceduralLandscapeComponent::GenerateMeshSection(TArray<FVector> Vertices, TArray<int32> Indices, TArray<FVector2D> UVs, FVector InSectionLocation, float InSectionHalfSize) const
{

}

void UProceduralLandscapeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RootSection->UpdateSection();
}







UProcLandscapeOctreeElem::~UProcLandscapeOctreeElem()
{
	ClearChilds();
	ClearMesh();
}

void UProcLandscapeOctreeElem::Initialize(UProceduralLandscapeComponent* InParentComponent, FVector WorldLocation, float HalfExtend, float HalfHeight, int32 InSectionLevel)
{
	ParentLandscapeComponent = InParentComponent;
	SectionLocation = WorldLocation;
	sectionBounds = FBoxSphereBounds(WorldLocation, FVector(HalfExtend, HalfExtend, HalfHeight), sqrt(HalfExtend * HalfExtend + HalfExtend * HalfExtend));
	sectionLevel = InSectionLevel;
}

float UProcLandscapeOctreeElem::GetScreenSize() const
{
	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(ParentLandscapeComponent->GetWorld(), 0))
	{
		return sectionBounds.SphereRadius / ((sectionBounds.Origin - CameraManager->GetCameraLocation()).Size() * sin(CameraManager->GetFOVAngle()));
	}
	return 0.f;
}

void UProcLandscapeOctreeElem::UpdateSection()
{
	if (GetParentComponent()->bDrawDebugs)
	{
		DebugDrawSection();
	}

	if (GetLevel() < GetLevelFromScreenSize(GetScreenSize()))
	{
		ClearMesh();
		GenerateChilds();
	}
	else
	{
		ClearChilds();
		GenerateMesh();
	}

	double time = UKismetSystemLibrary::GetGameTimeInSeconds(GetParentComponent()->GetWorld());

// 	if (assignedMeshSection != -1)
// 	{
// 		{
// 			SCOPE_CYCLE_COUNTER(STAT_TerrainUpdate);
// 			for (int i = 0; i < GetParentComponent()->MeshResolution * GetParentComponent()->MeshResolution * 4; ++i)
// 			{
// 				VerticesCopy[i] = Vertices[i] + FVector(0, 0, sin(time + Vertices[i].X / 500.f + Vertices[i].Y / 500.f) * 200);
// 			}
// 		}
// 
// 		SCOPE_CYCLE_COUNTER(STAT_TerrainInfoUpdate);
// 		GetParentComponent()->GetMesh()->UpdateMeshSection(assignedMeshSection, VerticesCopy, Normals, UVs, UVs1, UVs2, UVs3, VertexColors, Tangents);
// 	}


	for (auto& childSection : childs)
	{
		childSection->UpdateSection();
	}
}

void UProcLandscapeOctreeElem::GenerateChilds()
{
	if (childs.Num() > 0) return;
	if (assignedMeshSection != -1) return;

	UProcLandscapeOctreeElem* NewSection;

	NewSection = NewObject<UProcLandscapeOctreeElem>(this, ParentLandscapeComponent->OctreeSectionClass);
	NewSection->Initialize(GetParentComponent(), SectionLocation + FVector(GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.X / 2.f, 0), GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.Z, GetLevel() + 1);
	childs.Add(NewSection);

	NewSection = NewObject<UProcLandscapeOctreeElem>(this, ParentLandscapeComponent->OctreeSectionClass);
	NewSection->Initialize(GetParentComponent(), SectionLocation + FVector(-GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.X / 2.f, 0), GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.Z, GetLevel() + 1);
	childs.Add(NewSection);

	NewSection = NewObject<UProcLandscapeOctreeElem>(this, ParentLandscapeComponent->OctreeSectionClass);
	NewSection->Initialize(GetParentComponent(), SectionLocation + FVector(-GetSectionBounds().BoxExtent.X / 2.f, -GetSectionBounds().BoxExtent.X / 2.f, 0), GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.Z, GetLevel() + 1);
	childs.Add(NewSection);

	NewSection = NewObject<UProcLandscapeOctreeElem>(this, ParentLandscapeComponent->OctreeSectionClass);
	NewSection->Initialize(GetParentComponent(), SectionLocation + FVector(GetSectionBounds().BoxExtent.X / 2.f, -GetSectionBounds().BoxExtent.X / 2.f, 0), GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.Z, GetLevel() + 1);
	childs.Add(NewSection);
}

void UProcLandscapeOctreeElem::ClearChilds()
{
	if (childs.Num() <= 0) return;

	for (auto& childSection : childs)
	{
		if (childSection && childSection->IsValidLowLevel())
		{
			childSection->ClearMesh();
			childSection->ClearChilds();
			childSection->ConditionalBeginDestroy();
		}
	}
	childs = {};
}

void UProcLandscapeOctreeElem::GenerateMesh()
{
	if (assignedMeshSection != -1) return;
	if (childs.Num() > 0) return;

	SCOPE_CYCLE_COUNTER(STAT_TerrainGeneration);

	if (GetParentComponent()->FreeSections.Num() > 0)
	{
		assignedMeshSection = GetParentComponent()->FreeSections[0];
		GetParentComponent()->FreeSections.RemoveAt(0);
	}
	else
	{
		assignedMeshSection = GetParentComponent()->GetMesh()->GetNumSections();
	}


	int32 Resolution = GetParentComponent()->MeshResolution;
	float sectionScale = GetSectionBounds().BoxExtent.X * 2;
	float unitScale = sectionScale / Resolution;

	float minZ = 0, maxZ = 0;


	FVector CurrentLocation = SectionLocation - GetSectionBounds().BoxExtent.X;

	for (int32 x = 0; x < Resolution; ++x)
	{
		float posX = unitScale * x + CurrentLocation.X;

		for (int32 y = 0; y < Resolution; ++y)
		{
			float posY = unitScale * y + CurrentLocation.Y;
			
			float p0Altitude = GetAltitudeAtPoint(posX, posY);
			float p1Altitude = GetAltitudeAtPoint(posX + unitScale, posY);
			float p2Altitude = GetAltitudeAtPoint(posX, posY + unitScale);
			float p3Altitude = GetAltitudeAtPoint(posX + unitScale, posY + unitScale);

			if (p0Altitude < minZ || (x == 0 && y == 0))
				minZ = p0Altitude;
			else if (p1Altitude < minZ)
				minZ = p1Altitude;
			else if (p2Altitude < minZ)
				minZ = p2Altitude;
			else if (p3Altitude < minZ)
				minZ = p3Altitude;

			if (p0Altitude > maxZ || (x == 0 && y == 0))
				maxZ = p0Altitude;
			else if (p1Altitude > maxZ)
				maxZ = p1Altitude;
			else if (p2Altitude > maxZ)
				maxZ = p2Altitude;
			else if (p3Altitude > maxZ)
				maxZ = p3Altitude;

			Vertices.Add(FVector(posX, posY, p0Altitude));
			Vertices.Add(FVector(posX + unitScale, posY, p1Altitude));
			Vertices.Add(FVector(posX, posY + unitScale, p2Altitude));
			Vertices.Add(FVector(posX + unitScale, posY + unitScale, p3Altitude));

			UVs.Add(FVector2D(posX, posY) / 100.f);
			UVs.Add(FVector2D(posX + unitScale, posY) / 100.f);
			UVs.Add(FVector2D(posX, posY + unitScale) / 100.f);
			UVs.Add(FVector2D(posX + unitScale, posY + unitScale) / 100.f);

			Triangles.Add((x * Resolution + y) * 4 + 0);
			Triangles.Add((x * Resolution + y) * 4 + 2);
			Triangles.Add((x * Resolution + y) * 4 + 1);

			Triangles.Add((x * Resolution + y) * 4 + 1);
			Triangles.Add((x * Resolution + y) * 4 + 2);
			Triangles.Add((x * Resolution + y) * 4 + 3);
		}
	}
	VerticesCopy = Vertices;
	sectionBounds.Origin.Z = (minZ + maxZ) / 2.f;
	sectionBounds.BoxExtent.Z = abs(minZ - maxZ) / 2.f;
	sectionBounds.SphereRadius = sqrt(sectionBounds.BoxExtent.X * sectionBounds.BoxExtent.X + sectionBounds.BoxExtent.Y * sectionBounds.BoxExtent.Y + sectionBounds.BoxExtent.Z * sectionBounds.BoxExtent.Z);
	GetParentComponent()->GetMesh()->CreateMeshSection(assignedMeshSection, Vertices, Triangles, Normals, UVs, UVs1, UVs2, UVs3, VertexColors, Tangents, false);
	GetParentComponent()->GetMesh()->SetMaterial(assignedMeshSection, GetParentComponent()->LandscapeMaterial);
}

void UProcLandscapeOctreeElem::ClearMesh()
{
	if (assignedMeshSection <= -1) return;
	if (GetParentComponent() && GetParentComponent()->IsValidLowLevel() && GetParentComponent()->GetMesh() && GetParentComponent()->GetMesh()->IsValidLowLevel())
	{
		GetParentComponent()->GetMesh()->ClearMeshSection(assignedMeshSection);
		GetParentComponent()->FreeSections.Add(assignedMeshSection);
		assignedMeshSection = -1;
	}
}

float UProcLandscapeOctreeElem::GetLevelFromScreenSize_Implementation(float distance) const
{
	return UKismetMathLibrary::FClamp(GetParentComponent()->LodLevels - UKismetMathLibrary::FClamp((distance - 1.f) * GetParentComponent()->LodScale * -1, 0.f, 1.f) * GetParentComponent()->LodLevels, 0, GetParentComponent()->LodLevels);
}

void UProcLandscapeOctreeElem::DebugDrawSection_Implementation() const
{
	UKismetSystemLibrary::DrawDebugBox(ParentLandscapeComponent->GetWorld(), sectionBounds.Origin, FVector(sectionBounds.BoxExtent.X, sectionBounds.BoxExtent.Y, sectionBounds.BoxExtent.Z) , FColor::Red, FRotator::ZeroRotator, 0.f, 5.f);
}

float UProcLandscapeOctreeElem::GetAltitudeAtPoint_Implementation(const float& posX, const float& posY) const
{
	return USimplexNoiseBPLibrary::SimplexNoise2D(posX * 0.0002, posY * 0.0002) * 400.f + SectionLocation.Z;
}
