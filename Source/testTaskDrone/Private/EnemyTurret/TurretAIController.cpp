#include "EnemyTurret/TurretAIController.h"
#include "EnemyTurret/GunTurretPawn.h"
#include "PlayerDrone/PlayerDronePawn.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

ATurretAIController::ATurretAIController() : AAIController()
{
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATurretAIController::OnTargetPerceptionUpdated);
}


void ATurretAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

	UBlackboardComponent* Throwaway = nullptr; // we won't need this as we have GetBlackboardComponent() func anyway
	UseBlackboard(BehaviorTree->BlackboardAsset, Throwaway);
    if (BehaviorTree) RunBehaviorTree(BehaviorTree);
}

void ATurretAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;

	auto BlackboardComponent = GetBlackboardComponent();
	if (BlackboardComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get blackboard component"));
		return;
	}

	auto Player = Cast<APlayerDronePawn>(Actor);

	if (Stimulus.WasSuccessfullySensed() && Player && Player->IsDead() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player detected"));
		BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);
	}
	else
	{
		BlackboardComponent->ClearValue(TEXT("TargetActor"));
	}
}