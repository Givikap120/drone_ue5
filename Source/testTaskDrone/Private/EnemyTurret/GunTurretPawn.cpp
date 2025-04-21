#include "EnemyTurret/GunTurretPawn.h"
#include "AIController.h"
#include "BrainComponent.h"

// Sets default values
AGunTurretPawn::AGunTurretPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(BaseMesh);

	ShootingComponent = CreateDefaultSubobject<UShootingComponent>(TEXT("ShootingComponent"));
}

// Called every frame
void AGunTurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDestroyed) return;
	if (CurrentYaw == DesiredYaw && DesiredPitch == CurrentPitch) return;

	float DeltaYaw = FMath::Abs(FMath::FindDeltaAngleDegrees(DesiredYaw, CurrentYaw));
	float DeltaPitch = FMath::Abs(FMath::FindDeltaAngleDegrees(DesiredPitch, CurrentPitch));

	float DeltaTotal = FMath::Sqrt(DeltaYaw * DeltaYaw + DeltaPitch * DeltaPitch);
	if (DeltaTotal == 0) return; // extra check in case previous equality check failed because 0 != 360

	float MaxDeltaTotal = RotationSpeed * DeltaTime;
	float PercentageOfMovement = FMath::Clamp(MaxDeltaTotal / DeltaTotal, 0.0f, 1.0f);

	CurrentYaw = FMath::FixedTurn(CurrentYaw, DesiredYaw, DeltaYaw * PercentageOfMovement);
	CurrentPitch = FMath::FixedTurn(CurrentPitch, DesiredPitch, DeltaPitch * PercentageOfMovement);
	
	RotateSkeletalGunMesh(CurrentYaw, CurrentPitch);
}

void AGunTurretPawn::AimTo(const FVector& TargetLocation)
{
	if (IsDestroyed) return;

	LastAimLocation = TargetLocation;

	FVector OriginLocation = GunMesh->GetSocketLocation(FName("rail_02"));
	FVector Direction = (TargetLocation - OriginLocation).GetSafeNormal();

	FRotator AbsoluteRotation = Direction.Rotation();
	FRotator ActorRotation = GetActorRotation();

	DesiredYaw = FMath::UnwindDegrees(-90 - (ActorRotation.Yaw - AbsoluteRotation.Yaw));
	DesiredPitch = FMath::UnwindDegrees(ActorRotation.Pitch - AbsoluteRotation.Pitch);
}

bool AGunTurretPawn::Shoot_Implementation()
{
	if (IsDestroyed || !ShootingComponent) return false;
	return ShootingComponent->Shoot();
}

AActor* AGunTurretPawn::GetCurrentBarrelTarget()
{
	if (IsDestroyed || !ShootingComponent) return nullptr;

	FHitResult Hit = ShootingComponent->TraceShot();
	return Hit.GetActor();
}

void AGunTurretPawn::OnDeath_Implementation()
{
	IsDestroyed = true;

	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->BrainComponent->StopLogic("Turret destroyed");
		AI->UnPossess();
	}

	if (GunMesh) GunMesh->DestroyComponent();
	if (ShootingComponent) ShootingComponent->DestroyComponent();
}

FVector AGunTurretPawn::GetBarrelLocation_Implementation() const
{
	return GunMesh->GetSocketLocation(FName("barrel"));
}

FRotator AGunTurretPawn::GetBarrelDirection_Implementation() const
{
	auto Rotation = GunMesh->GetSocketRotation(FName("barrel"));

	// Offset rotation
	Rotation.Yaw = Rotation.Yaw + 90; 
	Rotation.Pitch = -Rotation.Roll + 90;

	return Rotation;
}