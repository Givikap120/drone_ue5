#include "Collectables/HealthCollectable.h"

void AHealthCollectable::Collect(APlayerDronePawn* Actor)
{
	bool result = Actor->TryRestoreHealth(HealthRestored);
	if (result) ACollectable::Collect(Actor);
}
