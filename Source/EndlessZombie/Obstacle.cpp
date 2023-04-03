// Copyright Jorge Kuijper. All Rights Reserved.


#include "Obstacle.h"
#include "ZombieCharacter.h"

// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->SetRelativeRotation(FRotator(0.f, 90.0f, 0.f));

	ObstacleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ObstacleCollision"));
	ObstacleCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ObstacleCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	ObstacleCollision->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::CollisionBeginOverlap);

}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::CollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsCollisioned) {
		if (OtherActor->IsA(AZombieCharacter::StaticClass()))
		{
			AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OtherActor);
			ZombieCharacter->ObstacleCollision();
			bIsCollisioned = true;
			StaticMesh->SetCollisionProfileName("NoCollision");
		}
	}
}

