#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICanShoot.generated.h"

UINTERFACE(MinimalAPI)
class UCanShoot : public UInterface
{
	GENERATED_BODY()
};

class TESTTASKDRONE_API ICanShoot
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooting")
	FVector GetBarrelLocation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooting")
	FRotator GetBarrelDirection() const;
};