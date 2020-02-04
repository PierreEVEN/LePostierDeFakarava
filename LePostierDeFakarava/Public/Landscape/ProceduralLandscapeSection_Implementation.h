
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralLandscapeModule/Mesh/ProceduralLandscapeSection.h"
#include "ProceduralLandscapeSection_Implementation.generated.h"

class ProceduralLandscapeOctreeNode;

class FProceduralLandscapeSectionImplementation : public ProceduralLandscapeSection
{
public:

	FProceduralLandscapeSectionImplementation(AProceduralLandscapeWrapper* wrapper);

	AProceduralLandscapeWrapper* parentWrapper;

	virtual unsigned int GetMaxLODLevel() const override;

	virtual ProceduralLandscapeOctreeNode* GenerateOctreeNode(const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel) override;

};

struct FUnrealFormatedSectionData;
class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class LEPOSTIERDEFAKARAVA_API AProceduralLandscapeWrapper : public AActor
{
	GENERATED_BODY()

public:

	AProceduralLandscapeWrapper(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	FProceduralLandscapeSectionImplementation* rootLandscapeSection;

	void AddMeshSection(FUnrealFormatedSectionData& sectionData);

	UPROPERTY(EditAnywhere, Category = "Parameters")
		float baseSectionHalfSize = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int32 maxLodCount = 5;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int32 NodeMeshResolution = 50;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		bool bEnableAsyncMeshGeneration = true;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		UMaterialInterface* LandscapeMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* LandscapeMesh = nullptr;



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float GetAltitudeAtLocation(float posX, float posY);
	float GetAltitudeAtLocation_Implementation(float posX, float posY) { return 0.f; }
};