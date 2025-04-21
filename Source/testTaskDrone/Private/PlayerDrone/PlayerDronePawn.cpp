#include "PlayerDrone/PlayerDronePawn.h"
#include "PlayerDrone/PlayerDroneController.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "EnemyTurret/GunTurretPawn.h"

APlayerDronePawn::APlayerDronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));

	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;

	ShootingComponent = CreateDefaultSubobject<UShootingComponent>(TEXT("ShootingComponent"));
}

void APlayerDronePawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	};
}

void APlayerDronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerDronePawn::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerDronePawn::Look);
		Input->BindAction(ShootAction, ETriggerEvent::Started, this, &APlayerDronePawn::Shoot);
		Input->BindAction(RespawnAction, ETriggerEvent::Started, this, &APlayerDronePawn::RequestRespawn);
	}
}

void APlayerDronePawn::AddAmmo(int Ammo)
{
	if (ShootingComponent) ShootingComponent->AddCurrentAmmo(Ammo);
}

void APlayerDronePawn::Move(const FInputActionValue& Value)
{
	if (IsDead()) return;

	const FVector Direction = Value.Get<FVector>();

	AddMovementInput(GetActorForwardVector(), Direction.X);
	AddMovementInput(GetActorRightVector(), Direction.Y);
	AddMovementInput(GetActorUpVector(), Direction.Z);
}

void APlayerDronePawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(-LookValue.Y);
}

void APlayerDronePawn::Shoot(const FInputActionValue& Value)
{
	if (IsDead()) return;
	if (ShootingComponent) ShootingComponent->Shoot();
}

void APlayerDronePawn::RequestRespawn(const FInputActionValue& Value)
{
	if (APlayerDroneController* DroneController = Cast<APlayerDroneController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to respawn"));
		DroneController->Respawn();
	}
}

FVector APlayerDronePawn::GetBarrelLocation_Implementation() const
{
	auto Location = GetActorLocation();

	// Spawn in front
	auto Direction = GetControlRotation().Vector();
	float radius = 0.0f;

	if (CollisionComponent) radius += CollisionComponent->GetScaledSphereRadius();
	if (ShootingComponent) radius += ShootingComponent->ProjectileSize;

	return Location + Direction * radius;
}

FRotator APlayerDronePawn::GetBarrelDirection_Implementation() const
{
	return GetControlRotation();
}

void APlayerDronePawn::OnDeath_Implementation()
{
	if (ShootingComponent) ShootingComponent->DestroyComponent();
	if (MovementComponent) MovementComponent->DestroyComponent();
}
