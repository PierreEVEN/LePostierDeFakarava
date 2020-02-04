// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FoliageSection.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;

USTRUCT(BlueprintType)
struct LEPOSTIERDEFAKARAVA_API FProceduralLandscapeFoliageParameters
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMesh* foliageMesh = nullptr;

	/** Foliage density (instance / cm) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 foliageDensity = 0.0001f;

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
		float minInstanceSize = .8f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float maxInstanceSize = 1.2f;
};

USTRUCT()
struct LEPOSTIERDEFAKARAVA_API FFoliageSectionInfos
{
	GENERATED_BODY()

public:
	FFoliageSectionInfos() {}

	FFoliageSectionInfos(FVector inSectionLocation, float inSectionHalfHeight)
		: sectionLocation(inSectionLocation), sectionHalfHeight(inSectionHalfHeight) {
	}

	UPROPERTY()
		FVector sectionLocation;

	UPROPERTY()
		float sectionHalfHeight;

	bool operator==(const FFoliageSectionInfos& other) const
	{
		return other.sectionLocation == sectionLocation && other.sectionHalfHeight == sectionHalfHeight;
	}
};

UCLASS()
class LEPOSTIERDEFAKARAVA_API UFoliageSection : public UObject
{
	GENERATED_BODY()
	
public:
	UFoliageSection() {}
	UPROPERTY()
		UHierarchicalInstancedStaticMeshComponent* meshComponent;

	UFUNCTION()
		void UpdateSection();

	UFUNCTION()
		void RemoveInstance(int32 instanceIndex);

	UFUNCTION()
		void AddSection(FFoliageSectionInfos inSection);

	UFUNCTION()
		void RemoveSection(FFoliageSectionInfos inSection);

};
