
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STypes.h"
#include "Mesh/ProceduralLandscapeSection.h"
#include "ProceduralLandscapeOctreeNode_Implementation.h"
#include "ProceduralLandscapeSection_Implementation.generated.h"

class ProceduralLandscapeOctreeNode;
class AProceduralLandscapeWrapper;

class LEPOSTIERDEFAKARAVA_API ProceduralLandscapeSectionImplementation : public ProceduralLandscapeSection
{
public:

	ProceduralLandscapeSectionImplementation(AProceduralLandscapeWrapper* wrapper, FProceduralLandscapeVector sectionPosition);

	AProceduralLandscapeWrapper* parentWrapper;


	virtual std::vector<FProceduralFoliageType> GetFoliageTypes() const override;

	virtual ProceduralLandscapeOctreeNode* GenerateOctreeNode(const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel) override
	{
		return new ProceduralLandscapeOctreeNodeImplementation(this, nodeLocation, halfSize, lodLevel);
	}

};

struct FUnrealFormatedSectionData;
class UProceduralMeshComponent;
class UMaterialInterface;

USTRUCT(BlueprintType)
struct LEPOSTIERDEFAKARAVA_API FUnrealFormatedFoliageType
{
	GENERATED_BODY()
public:


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMesh* mesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 density = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float randomisationRatio = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 minSpawnLodLevel = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 maxSpawnLodLevel = 5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float minAltitude = 20.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float maxAltitude = 2000.f;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float minNormalAngle = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float maxNormalAngle = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float minSize = 0.8f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float maxSize = 1.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAlignOnSurface = false;

	FProceduralFoliageType ToFoliageType() const
	{
		FProceduralFoliageType result;

		result.density = density;
		result.randomisationRatio = randomisationRatio;
		result.minSpawnLodLevel = minSpawnLodLevel;
		result.maxSpawnLodLevel = maxSpawnLodLevel;
		result.minAltitude = minAltitude;
		result.maxAltitude = maxAltitude;
		result.minNormalAngle = minNormalAngle;
		result.maxNormalAngle = maxNormalAngle;
		result.minSize = maxSize;
		result.bAlignOnSurface = bAlignOnSurface;

		return result;
	}

};


UCLASS()
class LEPOSTIERDEFAKARAVA_API AProceduralLandscapeWrapper : public AActor
{
	GENERATED_BODY()

public:

	AProceduralLandscapeWrapper(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	ProceduralLandscapeSectionImplementation* rootLandscapeSection = nullptr;

	int32 AddMeshSection(const FUnrealFormatedSectionData& sectionData);

	void RemoveMeshSection(int32 sectionIndice);

	UPROPERTY(EditAnywhere, Category = "Parameters")
		float baseSectionHalfSize = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int32 maxLodCount = 5;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		float maxSectionScreenSize = 0.75f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int32 NodeMeshResolution = 50;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		bool bEnableAsyncMeshGeneration = true;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		bool bGenerateCollisions = false;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		TArray<FUnrealFormatedFoliageType> foliages = {};

	UPROPERTY(EditAnywhere, Category = "Parameters")
		UMaterialInterface* LandscapeMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* LandscapeMesh = nullptr;

	TArray<int32> FreeSections;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float GetAltitudeAtLocation(float posX, float posY);
	float GetAltitudeAtLocation_Implementation(float posX, float posY) { return 0.f; }

	UFUNCTION()
		static AProceduralLandscapeWrapper* GetLandscape();

	UFUNCTION()
		static bool IsLocationUnderLandscape(FVector position);
};