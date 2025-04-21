#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/SphereComponent.h>
#include "BulletProjectile.generated.h"

UCLASS()
class TESTTASKDRONE_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletProjectile();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UParticleSystem* ImpactEffect;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetSpeed(float Speed);

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	float Damage = 0.0f;
};
