#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <EnhancedInputLibrary.h>
#include <GameFramework/FloatingPawnMovement.h>
#include <Components/SphereComponent.h>
#include "Shooting/ShootingComponent.h"
#include "Shooting/PawnWithHealth.h"
#include "Shooting/ICanShoot.h"

#include "PlayerDronePawn.generated.h"

UCLASS()
class TESTTASKDRONE_API APlayerDronePawn : public APawnWithHealth, public ICanShoot
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerDronePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UShootingComponent* ShootingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* RespawnAction;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AddAmmo(int Ammo);

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void RequestRespawn(const FInputActionValue& Value);

	virtual FVector GetBarrelLocation_Implementation() const override;
	virtual FRotator GetBarrelDirection_Implementation() const override;

	virtual void OnDeath_Implementation() override;
};
