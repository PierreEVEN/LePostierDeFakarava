// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../../../Public/Landscape/ProceduralLandscapeModule/Mesh/ProceduralLandscapeOctreeNode.h"
#include "MemPro/MemPro.h"

ProceduralLandscapeOctreeNode::ProceduralLandscapeOctreeNode(const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel)
	: NodePosition(nodeLocation), NodeHalfSize(halfSize), NodeLODLevel(lodLevel)
{

}

ProceduralLandscapeOctreeNode::~ProceduralLandscapeOctreeNode()
{
	if (meshGenerationAsyncThread && meshGenerationAsyncThread->joinable())
	{
		meshGenerationAsyncThread->join();
		meshGenerationAsyncThread = nullptr;
	}
}

void ProceduralLandscapeOctreeNode::Update()
{
	if (!bIsMeshDataGenerated && currentNodeStatus == NodeGenerationStatus::NodeGenerationStatus_Generated)
	{
		bIsMeshDataGenerated = true;
		OnMeshGenerated();
	}
}

void ProceduralLandscapeOctreeNode::RequestMeshGeneration()
{
	if (meshGenerationAsyncThread || currentNodeStatus != NodeGenerationStatus::NodeGenerationStatus_None) return;
	currentNodeStatus = NodeGenerationStatus::NodeGenerationStatus_WaitingForThreadStart;
	if (EnableAsyncMeshGeneration())
	{
		meshGenerationAsyncThread = new std::thread(GenerateMeshData, this);
	}
	else
	{
		GenerateMeshData(this);
	}
}

void ProceduralLandscapeOctreeNode::GenerateMeshData(ProceduralLandscapeOctreeNode* usedNode)
{
	usedNode->currentNodeStatus = NodeGenerationStatus::NodeGenerationStatus_Generating;
	


	int32 Resolution = (int32)usedNode->GetNodeMeshResolution();
	float sectionScale = usedNode->NodeHalfSize;
	float unitScale = sectionScale / Resolution;

	ProceduralLandscapeVector2D CurrentLocation = usedNode->NodePosition - ProceduralLandscapeVector2D(sectionScale);
	std::vector<ProceduralLandscapeLocationData> pointArrayData = {};
	
	/** Procedurally generate location map */
	for (int32 x = -1; x <= Resolution + 1; ++x)
	{
		float posX = unitScale * x + CurrentLocation.X;

		for (int32 y = -1; y <= Resolution + 1; ++y)
		{
			float posY = unitScale * y + CurrentLocation.Y;
			ProceduralLandscapeLocationData localData;
			usedNode->GetInformationsAtLocation(posX, posY, localData);
			pointArrayData.push_back(localData);
		}
	}

	/** Generate normals, tangents, UVs, positions and triangles */
	for (int32 x = 0; x < Resolution; ++x)
	{
		for (int32 y = 0; y < Resolution; ++y)
		{
			for (int i = 0; i < 4; ++i)
			{
				int xOffset = i == 1 || i == 3 ? 1 : 0;
				int yOffset = i == 2 || i == 3 ? 1 : 0;

				float posX = unitScale * (x + xOffset) + CurrentLocation.X;
				float posY = unitScale * (y + yOffset) + CurrentLocation.Y;


				ProceduralLandscapeVector originPoint(0.f, 0.f, pointArrayData[(x + 1 + xOffset) * (Resolution + 3) + (y + 1 + yOffset)].altitude);
				ProceduralLandscapeVector ptXLocation(unitScale, 0, pointArrayData[(x + 2 + xOffset) * (Resolution + 3) + (y + 1 + yOffset)].altitude);
				ProceduralLandscapeVector ptYLocation(0, unitScale, pointArrayData[(x + 1 + xOffset) * (Resolution + 3) + (y + 2 + yOffset)].altitude);
				ProceduralLandscapeVector ptmXLocation(-unitScale, 0, pointArrayData[(x + xOffset)* (Resolution + 3) + (y + 1 + yOffset)].altitude);
				ProceduralLandscapeVector ptmYLocation(-unitScale, 0, pointArrayData[(x + 1 + xOffset) * (Resolution + 3) + (y + yOffset)].altitude);

				ProceduralLandscapeVector normX(ProceduralLandscapeVector::CrossProduct(ptXLocation - originPoint, ptYLocation - ptXLocation));
				ProceduralLandscapeVector normY(ProceduralLandscapeVector::CrossProduct(ptYLocation - originPoint, ptmXLocation - ptXLocation));
				ProceduralLandscapeVector normmX(ProceduralLandscapeVector::CrossProduct(ptmXLocation - originPoint, ptmYLocation - ptXLocation));
				ProceduralLandscapeVector normmY(ProceduralLandscapeVector::CrossProduct(ptmYLocation - originPoint, ptXLocation - ptXLocation));

				ProceduralLandscapeVector outLocation(posX, posY, pointArrayData[(x + 1 + xOffset) * (Resolution + 3) + (y + 1 + yOffset)].altitude);
				ProceduralLandscapeVector outNormal = normX + normY + normmX + normmY / 4;
				ProceduralLandscapeVector outTangent = ProceduralLandscapeVector(ProceduralLandscapeVector::CrossProduct(outNormal.Normalize(), ptYLocation - originPoint));
				ProceduralLandscapeVector2D outUV = ProceduralLandscapeVector2D(outLocation.X / 100.f, outLocation.Y / 100.f);

				usedNode->NodeMeshData.AddVertice(outLocation, outNormal, outUV, ProceduralLandscapeColor(), outTangent);

				usedNode->NodeMeshData.AddTriangle(
					(x * Resolution + y) * 4,
					(x * Resolution + y) * 4 + 2,
					(x * Resolution + y) * 4 + 1
				);

				usedNode->NodeMeshData.AddTriangle(
					(x * Resolution + y) * 4 + 1,
					(x * Resolution + y) * 4 + 2,
					(x * Resolution + y) * 4 + 3
				);
			}
		}
	}

	usedNode->PostVerticesCreation();

	usedNode->currentNodeStatus = NodeGenerationStatus::NodeGenerationStatus_Generated;
}