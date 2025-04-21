#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "HealthCollectable.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKDRONE_API AHealthCollectable : public ACollectable
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Collectable")
	float HealthRestored = 25.0f;

	virtual void Collect(APlayerDronePawn* Actor) override;
};
