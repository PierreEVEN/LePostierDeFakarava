// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "../ProceduralLandscapeVector.h"

class ProceduralLandscapeOctreeNode;

class ProceduralLandscapeSection
{
public:

	ProceduralLandscapeSection();

	ProceduralLandscapeVector landscapeSectionLocation;
	float landscapeSectionSize;
	ProceduralLandscapeOctreeNode* parentOctreeNode = nullptr;


	virtual unsigned int GetMaxLODLevel() const = 0;

	virtual ProceduralLandscapeOctreeNode* GenerateOctreeNode(const ProceduralLandscapeVector2D& nodeLocation, const float& halfSize, const unsigned int& lodLevel) { return nullptr; }

	virtual void UpdateSection();
};
