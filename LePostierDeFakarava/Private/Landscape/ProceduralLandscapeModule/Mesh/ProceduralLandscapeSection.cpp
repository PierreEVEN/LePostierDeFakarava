// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../../../Public/Landscape/ProceduralLandscapeModule/Mesh/ProceduralLandscapeSection.h"
#include "Landscape/ProceduralLandscapeModule/Mesh/ProceduralLandscapeOctreeNode.h"

ProceduralLandscapeSection::ProceduralLandscapeSection()
{
}


void ProceduralLandscapeSection::UpdateSection()
{
	if (parentOctreeNode)
	{
		parentOctreeNode->Update();
	}
}
