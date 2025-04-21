#include "Collectables/Collectable.h"
#include <Kismet/GameplayStatics.h>

ACollectable::ACollectable()
{
	PrimaryActorTick.bCanEverTick = true;

    // Collision
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(50.0f);
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = Collision;

    // Mesh
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Bind overlap
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnOverlapBegin);
}

void ACollectable::BeginPlay()
{
	Super::BeginPlay();
    InitialLocation = GetActorLocation();
}

void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Floating animation
    CurrentAnimationTime += DeltaTime;

    float Phase = (CurrentAnimationTime / AnimationTime) * 2 * PI;
    float FloatOffset = FMath::Sin(Phase) * AnimationAmplitude;

    FVector NewLocation = InitialLocation;
    NewLocation.Z += FloatOffset;
    SetActorLocation(NewLocation);

    float RotationSpeed = 360.0f / AnimationTime;
    AddActorLocalRotation(FRotator(0, RotationSpeed * DeltaTime, 0));
}

void ACollectable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor->IsA<APlayerDronePawn>())
    {
        Collect(Cast<APlayerDronePawn>(OtherActor));
    }
}

void ACollectable::Collect(APlayerDronePawn* Actor)
{
    if (CollectSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, CollectSound, GetActorLocation());
    }

    Destroy();
}

