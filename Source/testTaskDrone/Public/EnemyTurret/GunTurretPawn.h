// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnWithHealth.h"
#include "ShootingComponent.h"
#include "ICanShoot.h"
#include "GunTurretPawn.generated.h"

UCLASS()
class TESTTASKDRONE_API AGunTurretPawn : public APawnWithHealth, public ICanShoot
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGunTurretPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Skeletal Meshes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UShootingComponent* ShootingComponent;

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void AimTo(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Turret")
	bool Shoot();
	virtual bool Shoot_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	AActor* GetCurrentBarrelTarget();

	UFUNCTION(BlueprintImplementableEvent, Category = "Turret")
	void RotateSkeletalGunMesh(float Yaw, float Pitch);

	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	float RotationSpeed = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Debug")
	FVector LastAimLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Damage = 5.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool IsDestroyed = false;

private:

	float CurrentYaw = 0;
	float CurrentPitch = 0;

	float DesiredYaw;
	float DesiredPitch;

	virtual FVector GetBarrelLocation_Implementation() const override;
	virtual FRotator GetBarrelDirection_Implementation() const override;

	void OnDeath_Implementation() override;
};
