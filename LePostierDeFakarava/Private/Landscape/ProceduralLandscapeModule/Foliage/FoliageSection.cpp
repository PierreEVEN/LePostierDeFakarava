// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../../../Public/Landscape/ProceduralLandscapeModule/Foliage/FoliageSection.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

void UFoliageSection::UpdateSection()
{


	//for (int32 instanceIndex = 0; instanceIndex < )



}

void UFoliageSection::RemoveInstance(int32 instanceIndex)
{
	FTransform newInstanceTransform;
	meshComponent->GetInstanceTransform(meshComponent->GetInstanceCount() - 1, newInstanceTransform);
	meshComponent->UpdateInstanceTransform(instanceIndex, newInstanceTransform, false);
	meshComponent->RemoveInstance(meshComponent->GetInstanceCount() - 1);
}

void UFoliageSection::AddSection(FFoliageSectionInfos inSection)
{

}

void UFoliageSection::RemoveSection(FFoliageSectionInfos inSection)
{

}
