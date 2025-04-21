#include "PawnWithHealth.h"

// Sets default values
APawnWithHealth::APawnWithHealth()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APawnWithHealth::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = StartingHealth;
	OnHealthChanged(CurrentHealth);
}

// Called to bind functionality to input
void APawnWithHealth::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool APawnWithHealth::TryRestoreHealth(float Heal)
{
	if (CurrentHealth >= MaxHealth)
		return false;

	CurrentHealth += Heal;
	CurrentHealth = FMath::Min(CurrentHealth, MaxHealth);
	OnHealthChanged(CurrentHealth);

	return true;
}

void APawnWithHealth::DamageHealth(float Damage)
{
	if (IsDead()) return;

	float OldHealth = CurrentHealth;

	CurrentHealth -= Damage;
	CurrentHealth = FMath::Max(CurrentHealth, 0);

	UE_LOG(LogTemp, Warning, TEXT("[%s] Took %.2f damage. Health: %.2f -> %.2f"),
		*GetName(), Damage, OldHealth, CurrentHealth);
	
	OnHealthChanged(CurrentHealth);
	if (IsDead()) OnDeath();
}

void APawnWithHealth::OnDeath_Implementation()
{
}

float APawnWithHealth::GetCurrentHealth()
{
	return CurrentHealth;
}

bool APawnWithHealth::IsDead()
{
	return CurrentHealth <= 0;
}
