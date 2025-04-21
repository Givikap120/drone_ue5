#include "PlayerDrone/PlayerDroneController.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

void APlayerDroneController::Respawn()
{
	if (!IsLocalController()) return;

	UWorld* World = GetWorld();
	if (!World) return;

	auto GameMode = GetWorld()->GetAuthGameMode();
	TSubclassOf<APawn> PawnClass = GameMode->DefaultPawnClass;

	AActor* PlayerStart = GameMode->FindPlayerStart(this);
	if (!PlayerStart) return;

	APawn* CurrentPawn = GetPawn();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetPawn();
	APawn* NewPawn = World->SpawnActor<APawn>(PawnClass, PlayerStart->GetActorTransform(), SpawnParams);

	Possess(NewPawn);
	UE_LOG(LogTemp, Warning, TEXT("Respawn succesful"));

	CurrentPawn->Destroy();
}