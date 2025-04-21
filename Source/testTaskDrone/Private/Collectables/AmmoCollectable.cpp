#include "Collectables/AmmoCollectable.h"

void AAmmoCollectable::Collect(APlayerDronePawn* Actor)
{
	Actor->AddAmmo(AmmoRestored);
	ACollectable::Collect(Actor);
}
