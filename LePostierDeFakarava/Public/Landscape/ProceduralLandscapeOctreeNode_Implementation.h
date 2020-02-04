
#pragma once
#include "ProceduralLandscapeModule/Mesh/ProceduralLandscapeOctreeNode.h"
#include "ProceduralMeshComponent.h"

class FProceduralLandscapeSectionImplementation;

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

	FUnrealFormatedSectionData(const LandscapeSectionData& source)
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

class FProceduralLandscapeOctreeNodeImplementation : public ProceduralLandscapeOctreeNode
{
public:

	FProceduralLandscapeOctreeNodeImplementation(FProceduralLandscapeSectionImplementation* parent, const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel);

	FProceduralLandscapeSectionImplementation* parentSection;

	virtual unsigned int GetNodeMeshResolution() const override;
	virtual bool EnableAsyncMeshGeneration() const override;
	virtual void GetInformationsAtLocation(float posX, float posY, ProceduralLandscapeLocationData& data) override;
	virtual void PostVerticesCreation() override;
	virtual ProceduralLandscapeVector GetCameraLocation() const override {
		return ProceduralLandscapeVector(0, 0, 0);
	}

	FUnrealFormatedSectionData unrealFormatedData;

	virtual void OnMeshGenerated() override;
};

