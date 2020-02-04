// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FoliageManager.generated.h"

/**
 * 
 */
UCLASS()
class LEPOSTIERDEFAKARAVA_API UFoliageManager : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION()
		void UpdateFoliage();


};
