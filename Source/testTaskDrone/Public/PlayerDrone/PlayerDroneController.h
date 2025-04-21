#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerDroneController.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKDRONE_API APlayerDroneController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Other")
	void Respawn();
};
