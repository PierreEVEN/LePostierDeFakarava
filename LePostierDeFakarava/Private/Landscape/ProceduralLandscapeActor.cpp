// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../Public/Landscape/ProceduralLandscapeActor.h"
#include "ProceduralMeshComponent.h"

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







void UProcLandscapeOctreeElem::Initialize(UProceduralLandscapeComponent* InParentComponent, FVector WorldLocation, float HalfExtend, float HalfHeight, int32 InSectionLevel)
{
	ParentLandscapeComponent = InParentComponent;
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
	DebugDrawSection();

	if (GetLevel() < GetLevelFromScreenSize(GetScreenSize()))
	{
		GenerateChilds();
	}
	else
	{
		ClearChilds();
		GenerateMesh();
	}


	for (auto& childSection : childs)
	{
		childSection->UpdateSection();
	}
}

void UProcLandscapeOctreeElem::GenerateChilds()
{
	if (childs.Num() > 0) return;

	UProcLandscapeOctreeElem* NewSection;

	NewSection = NewObject<UProcLandscapeOctreeElem>(this, ParentLandscapeComponent->OctreeSectionClass);
	NewSection->Initialize(GetParentComponent(), GetSectionBounds().Origin + FVector(GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.X / 2.f, 0), GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.Z, GetLevel() + 1);
	childs.Add(NewSection);

	NewSection = NewObject<UProcLandscapeOctreeElem>(this, ParentLandscapeComponent->OctreeSectionClass);
	NewSection->Initialize(GetParentComponent(), GetSectionBounds().Origin + FVector(-GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.X / 2.f, 0), GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.Z, GetLevel() + 1);
	childs.Add(NewSection);

	NewSection = NewObject<UProcLandscapeOctreeElem>(this, ParentLandscapeComponent->OctreeSectionClass);
	NewSection->Initialize(GetParentComponent(), GetSectionBounds().Origin + FVector(-GetSectionBounds().BoxExtent.X / 2.f, -GetSectionBounds().BoxExtent.X / 2.f, 0), GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.Z, GetLevel() + 1);
	childs.Add(NewSection);

	NewSection = NewObject<UProcLandscapeOctreeElem>(this, ParentLandscapeComponent->OctreeSectionClass);
	NewSection->Initialize(GetParentComponent(), GetSectionBounds().Origin + FVector(GetSectionBounds().BoxExtent.X / 2.f, -GetSectionBounds().BoxExtent.X / 2.f, 0), GetSectionBounds().BoxExtent.X / 2.f, GetSectionBounds().BoxExtent.Z, GetLevel() + 1);
	childs.Add(NewSection);
}

void UProcLandscapeOctreeElem::ClearChilds()
{
	return;
	if (childs.Num() <= 0) return;

	for (auto& childSection : childs)
	{
		//childSection->Delete();
	}
	childs.Reset();
}

void UProcLandscapeOctreeElem::GenerateMesh()
{
	if (assignedMeshSection != -1) return;
	assignedMeshSection = GetParentComponent()->GetMesh()->GetNumSections();
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FVector2D> UVs1;
	TArray<FVector2D> UVs2;
	TArray<FVector2D> UVs3;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	FVector sectionLocation = GetSectionBounds().Origin - FVector(GetSectionBounds().BoxExtent.X);
	int32 Resolution = GetParentComponent()->MeshResolution;
	float sectionScale = GetSectionBounds().BoxExtent.X * 2;
	float unitScale = sectionScale / Resolution;

	for (int32 x = 0; x < Resolution; ++x)
	{
		float posX = unitScale * x + sectionLocation.X;

		for (int32 y = 0; y < Resolution; ++y)
		{
			float posY = unitScale * y + sectionLocation.Y;
			
			Vertices.Add(FVector(posX, posY, GetAltitudeAtPoint(posX, posY)));
			Vertices.Add(FVector(posX + unitScale, posY, GetAltitudeAtPoint(posX + unitScale, posY)));
			Vertices.Add(FVector(posX, posY + unitScale, GetAltitudeAtPoint(posX, posY + unitScale)));
			Vertices.Add(FVector(posX + unitScale, posY + unitScale, GetAltitudeAtPoint(posX + unitScale, posY + unitScale)));

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
	GEngine->AddOnScreenDebugMessage(rand(), 4.f, FColor::Red, "add section " + FString::FromInt(assignedMeshSection));
	GetParentComponent()->GetMesh()->CreateMeshSection(assignedMeshSection, Vertices, Triangles, Normals, UVs, UVs1, UVs2, UVs3, VertexColors, Tangents, false);
}

void UProcLandscapeOctreeElem::DebugDrawSection_Implementation() const
{
	UKismetSystemLibrary::DrawDebugBox(ParentLandscapeComponent->GetWorld(), sectionBounds.Origin, FVector(sectionBounds.BoxExtent.X, sectionBounds.BoxExtent.Y, 1.f) , FColor::Red, FRotator::ZeroRotator, 0.f, 5.f);
}
