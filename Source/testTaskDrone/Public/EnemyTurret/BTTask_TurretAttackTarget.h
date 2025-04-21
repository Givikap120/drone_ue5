#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GunTurretPawn.h"
#include "BTTask_TurretAttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKDRONE_API UBTTask_TurretAttackTarget : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
