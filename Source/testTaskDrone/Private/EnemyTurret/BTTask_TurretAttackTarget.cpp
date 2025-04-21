#include "EnemyTurret/BTTask_TurretAttackTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

static FVector PredictIntercept(FVector MuzzleLocation, float ProjectileSpeed, FVector TargetLocation, FVector TargetVelocity)
{
	FVector ToTarget = TargetLocation - MuzzleLocation;
	float A = TargetVelocity.SizeSquared() - FMath::Square(ProjectileSpeed);
	float B = 2 * FVector::DotProduct(ToTarget, TargetVelocity);
	float C = ToTarget.SizeSquared();

	float Discriminant = B * B - 4 * A * C;
	if (Discriminant < 0 || FMath::IsNearlyZero(A))
	{
		return FVector::ZeroVector; // Can't hit
	}

	float Time = (-B + FMath::Sqrt(Discriminant)) / (2 * A);
	return TargetLocation - TargetVelocity * Time;
}

float GetDistanceToLine(const FVector& LineOrigin, const FVector& LineDir, const FVector& Point)
{
	FVector ClosestPoint = FMath::ClosestPointOnInfiniteLine(LineOrigin, LineOrigin + LineDir, Point);
	return FVector::Dist(ClosestPoint, Point);
}

float GetTargetSize(AActor* Actor)
{
	if (!Actor) return 0.0f;

	if (const USceneComponent* Root = Actor->GetRootComponent())
	{
		return Root->Bounds.SphereRadius;
	}

	return 0.0f;
}

EBTNodeResult::Type UBTTask_TurretAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AGunTurretPawn* TurretPawn = Cast<AGunTurretPawn>(AIController->GetPawn());

	if (!TurretPawn) return EBTNodeResult::Failed;

	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return EBTNodeResult::Failed;

	UObject* TargetObject = BlackboardComponent->GetValueAsObject("TargetActor");
	APawnWithHealth* TargetActor = Cast<APawnWithHealth>(TargetObject);
	if (!TargetActor) return EBTNodeResult::Failed;

	// If target is already dead - remove it as target
	if (TargetActor->IsDead())
	{
		BlackboardComponent->ClearValue("TargetActor");
		return EBTNodeResult::Failed;
	}

	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector TargetVelocity = TargetActor->GetVelocity();
	FVector BarrelLocation = ICanShoot::Execute_GetBarrelLocation(TurretPawn);
	FVector BarrelDirection = ICanShoot::Execute_GetBarrelDirection(TurretPawn).Vector();
	const float ProjectileSpeed = TurretPawn->ShootingComponent->ProjectileSpeed;

	FVector AimLocation = PredictIntercept(
		BarrelLocation,
		ProjectileSpeed,
		TargetLocation,
		TargetVelocity
	);

	// If prediction failed - just aim to the target
	if (AimLocation.IsNearlyZero()) AimLocation = TargetLocation;
	TurretPawn->AimTo(AimLocation);

	float Distance = GetDistanceToLine(BarrelLocation, BarrelDirection, AimLocation);
	float AimTolerance = 0.5f * (TurretPawn->ShootingComponent->ProjectileSize + GetTargetSize(TargetActor));
	AimTolerance = FMath::Max(AimTolerance, 0.1); // Safeguard check

	if (Distance <= AimTolerance)
	{
		TurretPawn->Shoot();
	}

	return EBTNodeResult::Succeeded;
}