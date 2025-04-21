#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shooting/BulletProjectile.h"
#include "ShootingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAmmoChangedSignature, int, NewAmmo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTTASKDRONE_API UShootingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FAmmoChangedSignature OnAmmoChanged;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<ABulletProjectile> ProjectileClass = ABulletProjectile::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ShootCooldown = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int StartingAmmo = 25;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Damage = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ProjectileSpeed = 10000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ProjectileSize = 32.0f;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	FHitResult TraceShot();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool Shoot();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AddCurrentAmmo(int Ammo);

private:
	int CurrentAmmo;

	float LastShootTime = -1.0f;

	bool TryDamageTarget(const FHitResult& Hit) const;
};
