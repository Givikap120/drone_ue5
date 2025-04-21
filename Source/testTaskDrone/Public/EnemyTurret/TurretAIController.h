#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKDRONE_API ATurretAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATurretAIController();

	void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
