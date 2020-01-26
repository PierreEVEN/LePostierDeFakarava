// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralLandscapeActor.generated.h"

class UProceduralMeshComponent;

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
		TSubclassOf<UProcLandscapeOctreeElem> OctreeSectionClass;

	UFUNCTION(BlueprintCallable)
		void GenerateMeshSection(TArray<FVector> Vertices, TArray<int32> Indices, TArray<FVector2D> UVs, FVector InSectionLocation, float InSectionHalfSize) const;

	UFUNCTION(BlueprintPure)
		UProceduralMeshComponent* GetMesh() const { return LandscapeMesh; }

	UFUNCTION(BlueprintPure)
		UProcLandscapeOctreeElem* GetRootSection() const { return RootSection; }

private:

	UPROPERTY()
		UProceduralMeshComponent* LandscapeMesh = nullptr;

	UPROPERTY()
		UProcLandscapeOctreeElem* RootSection = nullptr;
};




UCLASS(BlueprintType, Blueprintable)
class LEPOSTIERDEFAKARAVA_API UProcLandscapeOctreeElem : public UObject
{
	GENERATED_BODY()
private:

	UPROPERTY()
		UProceduralLandscapeComponent* ParentLandscapeComponent;

	UPROPERTY()
		FBoxSphereBounds sectionBounds;

	UPROPERTY()
		TArray<UProcLandscapeOctreeElem*> childs;

	UPROPERTY()
		int32 assignedMeshSection = -1;

	UPROPERTY()
		int32 sectionLevel = -1;
public:

	UProcLandscapeOctreeElem() {}

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

	UFUNCTION(BlueprintCallable)
	void UpdateSection();

	UFUNCTION()
	void GenerateChilds();

	UFUNCTION()
	void ClearChilds();

	UFUNCTION()
		void GenerateMesh();

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		int32 GetLevelFromScreenSize(float distance);
	int32 GetLevelFromScreenSize_Implementation(float distance) const { return 0; }

	UFUNCTION(BlueprintNativeEvent)
		void DebugDrawSection();
	void DebugDrawSection_Implementation() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		float GetAltitudeAtPoint(const float& posX, const float& posY) const;
	float GetAltitudeAtPoint_Implementation(const float& posX, const float& posY) const { return 0.f; }

};