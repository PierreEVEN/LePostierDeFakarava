// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once
#include <vector>
#include <thread>
#include "../ProceduralLandscapeVector.h"

enum class NodeGenerationStatus
{
	NodeGenerationStatus_None,
	NodeGenerationStatus_WaitingForThreadStart,
	NodeGenerationStatus_Generating,
	NodeGenerationStatus_Generated
};

struct ProceduralLandscapeLocationData
{
	float altitude;
	float temperature;
};

struct LandscapeSectionData
{
public:
	std::vector<ProceduralLandscapeVector> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<ProceduralLandscapeVector> Normals;
	std::vector<ProceduralLandscapeVector2D> UVs;
	std::vector<ProceduralLandscapeColor> VertexColors;
	std::vector<ProceduralLandscapeVector> Tangents;

	LandscapeSectionData() {}

	void AddVertice(const ProceduralLandscapeVector& location, const ProceduralLandscapeVector& normal, const ProceduralLandscapeVector2D& uvCoordinates, const ProceduralLandscapeColor& vertexColor, const ProceduralLandscapeVector& tangent) {
		Vertices.push_back(location);
		Normals.push_back(normal);
		UVs.push_back(uvCoordinates);
		VertexColors.push_back(vertexColor);
		Tangents.push_back(tangent);
	}

	void AddTriangle(const unsigned int& pos1, const unsigned int& pos2, const unsigned int& pos3) {
		Indices.push_back(pos1);
		Indices.push_back(pos2);
		Indices.push_back(pos3);
	}

	void Clear() {
		Vertices.clear();
		Indices.clear();
		Normals.clear();
		UVs.clear();
		VertexColors.clear();
		Tangents.clear();
	}

	ProceduralLandscapeVector* GetVerticesData() { return Vertices.data(); }
	unsigned int* GetIndicesData() { return Indices.data(); }
	ProceduralLandscapeVector* GetNormalsData() { return Normals.data(); }
	ProceduralLandscapeVector2D* GetUVsData() { return UVs.data(); }
	ProceduralLandscapeColor* GetVertexColorsData() { return VertexColors.data(); }
	ProceduralLandscapeVector* GetTangentsData() { return Tangents.data(); }

	unsigned int GetVerticesCount() const { return Vertices.size(); }
	unsigned int GetIndicesCount() const { return Indices.size(); }
	unsigned int GetNormalsCount() const { return Normals.size(); }
	unsigned int GetUVsCount() const { return UVs.size(); }
	unsigned int GetVertexColorsCount() const { return VertexColors.size(); }
	unsigned int GetTangentsCount() const { return Tangents.size(); }
};

class ProceduralLandscapeOctreeNode
{
protected:

	LandscapeSectionData NodeMeshData;

	ProceduralLandscapeVector2D NodePosition;
	float NodeHalfSize;
	unsigned int NodeLODLevel;

	std::thread* meshGenerationAsyncThread = nullptr;
	NodeGenerationStatus currentNodeStatus = NodeGenerationStatus::NodeGenerationStatus_None;

	bool bIsMeshDataGenerated = false;

public:

	ProceduralLandscapeOctreeNode(const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel);
	virtual ~ProceduralLandscapeOctreeNode();

	virtual unsigned int GetNodeMeshResolution() const = 0;

	void Update();

	void RequestMeshGeneration();

	virtual void OnMeshGenerated() = 0;
	virtual bool EnableAsyncMeshGeneration() const = 0;
	virtual void GetInformationsAtLocation(float posX, float posY, ProceduralLandscapeLocationData& data) = 0;

	virtual ProceduralLandscapeVector GetCameraLocation() const = 0;

	static void GenerateMeshData(ProceduralLandscapeOctreeNode* usedNode);
	virtual void PostVerticesCreation() {}
};
