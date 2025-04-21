#include "ShootingComponent.h"
#include "PawnWithHealth.h"
#include <Kismet/GameplayStatics.h>
#include "ICanShoot.h"

// Called when the game starts
void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = StartingAmmo;
	OnAmmoChanged.Broadcast(CurrentAmmo);
}

int UShootingComponent::GetCurrentAmmo() const 
{ 
	return CurrentAmmo; 
}

void UShootingComponent::AddCurrentAmmo(int Ammo)
{
	if (Ammo == 0) return;

	CurrentAmmo += Ammo;
	OnAmmoChanged.Broadcast(CurrentAmmo);
}

FHitResult UShootingComponent::TraceShot()
{
	AActor* Owner = GetOwner();
	if (!Owner || !Owner->Implements<UCanShoot>())
	{
		UE_LOG(LogTemp, Error, TEXT("Owner of ShootingComponent doesn't exist or not implements UCanShoot"));
		return FHitResult();
	}

	FVector Start = ICanShoot::Execute_GetBarrelLocation(Owner);
	FVector Direction = ICanShoot::Execute_GetBarrelDirection(Owner).Vector();
	FVector End = Start + Direction * 10000.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	return Hit;
}

bool UShootingComponent::Shoot()
{
	// Handle ammo
	if (CurrentAmmo <= 0) return false;

	// Handle rate of fire
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastShootTime < ShootCooldown) return false;

	if (!ProjectileClass) return false;

	LastShootTime = CurrentTime;

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());

	FVector SpawnLocation = ICanShoot::Execute_GetBarrelLocation(GetOwner());
	FRotator SpawnRotation = ICanShoot::Execute_GetBarrelDirection(GetOwner());

	ABulletProjectile* Projectile = GetWorld()->SpawnActor<ABulletProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, Params);

	Projectile->SetDamage(Damage);
	Projectile->SetSpeed(ProjectileSpeed);
	Projectile->Collision->SetSphereRadius(ProjectileSize);

	CurrentAmmo--;
	OnAmmoChanged.Broadcast(CurrentAmmo);

	if (ShootSound) UGameplayStatics::PlaySoundAtLocation(this, ShootSound, ICanShoot::Execute_GetBarrelLocation(GetOwner()));

	return true;
}

bool UShootingComponent::TryDamageTarget(const FHitResult& Hit) const
{
	if (AActor* HitActor = Hit.GetActor())
	{
		if (APawnWithHealth* Target = Cast<APawnWithHealth>(HitActor))
		{
			Target->DamageHealth(Damage);
			return true;
		}
	}

	return false;
}

