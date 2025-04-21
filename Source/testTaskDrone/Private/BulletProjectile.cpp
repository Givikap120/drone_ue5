#include "BulletProjectile.h"
#include <PawnWithHealth.h>
#include <Kismet/GameplayStatics.h>

ABulletProjectile::ABulletProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	Collision->OnComponentHit.AddDynamic(this, &ABulletProjectile::OnHit);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	Movement->bShouldBounce = false;
	Movement->ProjectileGravityScale = 0.0f;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetMobility(EComponentMobility::Movable);
	StaticMesh->SetAbsolute(false, false, false);
	StaticMesh->SetRelativeLocation(FVector::ZeroVector);
}

void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();

	AActor* InstigatorActor = GetInstigator();

	if (InstigatorActor && Collision)
	{
		Collision->IgnoreActorWhenMoving(InstigatorActor, true);
	}
}

void ABulletProjectile::SetDamage(float damage)
{
	Damage = damage;
}

void ABulletProjectile::SetSpeed(float speed)
{
	Movement->InitialSpeed = speed;
	Movement->MaxSpeed = speed;
	Movement->Velocity = GetActorForwardVector() * speed;
}

void ABulletProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor == GetOwner()) return;

	if (APawnWithHealth* Target = Cast<APawnWithHealth>(OtherActor))
	{
		Target->DamageHealth(Damage);
	}

	if (Hit.bBlockingHit && ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ImpactEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			true
		);
	}

	Destroy();
}

