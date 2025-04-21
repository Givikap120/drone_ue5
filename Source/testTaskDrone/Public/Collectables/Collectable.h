#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include <./PlayerDrone/PlayerDronePawn.h>

#include "Collectable.generated.h"

UCLASS()
class TESTTASKDRONE_API ACollectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* Collision;

    UPROPERTY(EditAnywhere, Category = "Collectable")
    USoundBase* CollectSound;

    UPROPERTY(EditDefaultsOnly, Category = "Collectable")
    float AnimationTime = 2.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Collectable")
    float AnimationAmplitude = 20.0f;

    // Overlap event
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void Collect(APlayerDronePawn* Actor);

private:
    FVector InitialLocation;
    float CurrentAnimationTime = 0;
};
