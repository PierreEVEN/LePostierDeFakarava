// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralLandscapeActor.generated.h"

class UProceduralMeshComponent;
class UProcLandscapeOctreeElem;

USTRUCT(BlueprintType)
struct LEPOSTIERDEFAKARAVA_API FProceduralLandscapeFoliageType
{
	GENERATED_BODY()

public:

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMesh* foliageMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 foliageDensity = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 foliageMinLOD = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 foliageMaxLOD = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float minAltitude = 2000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float maxAltitude = 100000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float randomness = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float minSize = .8f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float maxSize = 1.2f;
};


UCLASS()
class LEPOSTIERDEFAKARAVA_API AProceduralLandscapeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AProceduralLandscapeActor(const class FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UProceduralLandscapeComponent* rootTestComponent;

protected:
	virtual void BeginPlay() override;
};


UCLASS()
class LEPOSTIERDEFAKARAVA_API UProceduralLandscapeComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UProceduralLandscapeComponent(const class FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void AttachMeshComponent(const class FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Parameters")
		float BaseSectionHalfSize = 2000;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int32 MeshResolution = 20;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		float LodScale = 3.f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int32 LodLevels = 10;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		bool bDrawDebugs = false;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		UMaterialInterface* LandscapeMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		TSubclassOf<UProcLandscapeOctreeElem> OctreeSectionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FProceduralLandscapeFoliageType> landscapeFoliage;

	UFUNCTION(BlueprintCallable)
		void GenerateMeshSection(TArray<FVector> Vertices, TArray<int32> Indices, TArray<FVector2D> UVs, FVector InSectionLocation, float InSectionHalfSize) const;

	UFUNCTION(BlueprintPure)
		UProceduralMeshComponent* GetMesh() const { return LandscapeMesh; }

	UFUNCTION(BlueprintPure)
		UProcLandscapeOctreeElem* GetRootSection() const { return RootSection; }

	UPROPERTY()
		TArray<int32> FreeSections;

private:

	UPROPERTY()
		UProceduralMeshComponent* LandscapeMesh = nullptr;

	UPROPERTY()
		UProcLandscapeOctreeElem* RootSection = nullptr;
};

class UHierarchicalInstancedStaticMeshComponent;


USTRUCT(BlueprintType)
struct LEPOSTIERDEFAKARAVA_API FProceduralLandscapeVertice
{
	GENERATED_BODY()

public:

	FVector Vertice;
	int32 Triangles;
	FVector Normals;
	FVector2D UVs;
	FColor VertexColors;
	FProcMeshTangent Tangents;
};

UCLASS(BlueprintType, Blueprintable)
class LEPOSTIERDEFAKARAVA_API UProcLandscapeOctreeElem : public UObject
{
	GENERATED_BODY()
private:

	UPROPERTY()
		FVector SectionLocation;

	UPROPERTY()
		UProceduralLandscapeComponent* ParentLandscapeComponent;

	UPROPERTY()
		TArray<UHierarchicalInstancedStaticMeshComponent*> LandscapeFoliages;

	UPROPERTY()
		FBoxSphereBounds sectionBounds;

	UPROPERTY()
		TArray<UProcLandscapeOctreeElem*> childs;

	UPROPERTY()
		int32 assignedMeshSection = -1;

	UPROPERTY()
		int32 sectionLevel = -1;


	TArray<FVector> Vertices;
	TArray<FVector> VerticesCopy;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FVector2D> UVs1;
	TArray<FVector2D> UVs2;
	TArray<FVector2D> UVs3;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

public:

	UProcLandscapeOctreeElem() {}

	~UProcLandscapeOctreeElem();

	UFUNCTION()
	void Initialize(UProceduralLandscapeComponent* InParentComponent, FVector WorldLocation, float HalfExtend, float HalfHeight, int32 InSectionLevel);

	UFUNCTION(BlueprintPure)
	float GetScreenSize() const;

	UFUNCTION(BlueprintPure)
		int32 GetLevel() const { return sectionLevel; }

	UFUNCTION(BlueprintPure)
		UProceduralLandscapeComponent* GetParentComponent() const { return ParentLandscapeComponent; }

	UFUNCTION(BlueprintPure)
		FBoxSphereBounds GetSectionBounds() const { return sectionBounds; }

	UFUNCTION(BlueprintPure)
		FVector GetSectionLocation() const { return SectionLocation; }

	UFUNCTION(BlueprintCallable)
	void UpdateSection();

	UFUNCTION()
	void GenerateChilds();

	UFUNCTION()
	void ClearChilds();

	UFUNCTION()
		void GenerateMesh();

	UFUNCTION()
		void GenerateLandscapeFoliage();

	UFUNCTION()
		void CleanupLandscapeFoliage();

	UFUNCTION()
		void ClearMesh();

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		float GetLevelFromScreenSize(float distance);
	float GetLevelFromScreenSize_Implementation(float distance) const;

	UFUNCTION(BlueprintNativeEvent)
		void DebugDrawSection();
	void DebugDrawSection_Implementation() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		float GetAltitudeAtPoint(const float& posX, const float& posY) const;
	float GetAltitudeAtPoint_Implementation(const float& posX, const float& posY) const;

};