// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnWithHealth.generated.h"

UCLASS()
class TESTTASKDRONE_API APawnWithHealth : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnWithHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int StartingHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int MaxHealth = 200;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool TryRestoreHealth(float Heal);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DamageHealth(float Damage);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void OnHealthChanged(float HP);

	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void OnDeath();
	virtual void OnDeath_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsDead();

protected:
	float CurrentHealth;
};
