
#pragma once
#include "Mesh/ProceduralLandscapeOctreeNode.h"
#include "ProceduralMeshComponent.h"
#include "STypes.h"

class ProceduralLandscapeSectionImplementation;

struct FUnrealFormatedFoliageData
{
public:
	TArray<FTransform> transforms;
	UStaticMesh* meshs;
};

struct FUnrealFormatedSectionData
{
public:
	TArray<FVector> Vertices;
	TArray<int32> Indices;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	FUnrealFormatedSectionData() {}

	FUnrealFormatedSectionData(const FProceduralLandscapeNodeMeshData& source)
	{
		Vertices.Reset(source.Vertices.size());
		Indices.Reset(source.Indices.size());
		Normals.Reset(source.Normals.size());
		UVs.Reset(source.UVs.size());
		VertexColors.Reset(source.VertexColors.size());
		Tangents.Reset(source.Tangents.size());
		for (const auto& vertice : source.Vertices)
		{
			Vertices.Add(FVector(vertice.X, vertice.Y, vertice.Z));
		}
		for (const auto& indice : source.Indices)
		{
			Indices.Add(indice);
		}
		for (const auto& normal : source.Normals)
		{
			Normals.Add(FVector(normal.X, normal.Y, normal.Z));
		}
		for (const auto& uv : source.UVs)
		{
			UVs.Add(FVector2D(uv.X, uv.Y));
		}
		for (const auto& Color : source.VertexColors)
		{
			VertexColors.Add(FColor(Color.r, Color.g, Color.b, Color.a));
		}
		for (const auto& Tangent : source.Tangents)
		{
			Tangents.Add(FProcMeshTangent(FVector(Tangent.X, Tangent.Y, Tangent.Z), false));
		}
	}
};

class UHierarchicalInstancedStaticMeshComponent;

class LEPOSTIERDEFAKARAVA_API ProceduralLandscapeOctreeNodeImplementation : public ProceduralLandscapeOctreeNode
{
public:

	FUnrealFormatedSectionData unrealFormatedData;
	TArray<FUnrealFormatedFoliageData> unrealFormatedFoliages;


	int32 generatedSectionIndice = -1;
	TArray<UHierarchicalInstancedStaticMeshComponent*> foliageHisms;
	
	ProceduralLandscapeOctreeNodeImplementation(ProceduralLandscapeSection* inParentSection, const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel)
		: ProceduralLandscapeOctreeNode(inParentSection, nodeLocation, halfSize, lodLevel)
	{
		RequestNodeGeneration();
	}
	virtual ~ProceduralLandscapeOctreeNodeImplementation() {}
	ProceduralLandscapeSectionImplementation* GetParentSection() const { return (ProceduralLandscapeSectionImplementation*)parentSection; }
	virtual unsigned int GetNodeMeshResolution() const override;
	virtual unsigned int GetMaxLODLevel() const override;
	virtual float GetNodeMaxScreenSize() const override;

	virtual bool EnableAsyncGeneration() const override;
	virtual void GetInformationsAtLocation(float posX, float posY, ProceduralLandscapeLocationData& data) const override;
	virtual FProceduralLandscapeVector GetCameraLocation() const override;
	virtual float GetCameraFov() const override;

	virtual void PostVerticesCreation() override;
	virtual void PostFoliageCreation() override;
	virtual void OnMeshGenerated() override;
	virtual void OnFoliageGenerated() override;
	virtual void OnMeshRemoved() override;
	virtual void OnFoliageRemoved() override;
};

