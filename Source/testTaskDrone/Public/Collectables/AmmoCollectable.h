#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "AmmoCollectable.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKDRONE_API AAmmoCollectable : public ACollectable
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Collectable")
	int AmmoRestored = 10;

	virtual void Collect(APlayerDronePawn* Actor) override;
};
