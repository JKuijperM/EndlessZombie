// Copyright Jorge Kuijper. All Rights Reserved.


#include "Tile.h"
#include "ZombieCharacter.h"
#include "PathGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Add scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;
	//SceneComponent->SetupAttachment(RootComponent);
	// Add arrow component like attach point
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	ArrowComponent->SetupAttachment(SceneComponent);

	// Add box collider
	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndPoint"));
	EndTrigger->SetupAttachment(SceneComponent);
	EndTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	// Add the obstacle zone
	ObstacleZone = CreateDefaultSubobject<UBoxComponent>(TEXT("ObstacleZone"));
	ObstacleZone->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ObstacleZone->SetRelativeLocation(FVector(511.0f, 0.f, 26.0f));
	ObstacleZone->SetRelativeScale3D(FVector(12.50f, 14.5f, 1.0f));

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	EndTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATile::OnBeginOverlap);

	SpawnRandomLocation(ObstacleZone);

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ATile::GetAttachTransform()
{
	FTransform tAttachPoint;

	tAttachPoint = ArrowComponent->GetComponentTransform();

	return tAttachPoint;
}

void ATile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OtherActor);

	if (ZombieCharacter != nullptr)
	{
		TArray<AActor*> ActorsToFind;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APathGenerator::StaticClass(), ActorsToFind);
		if (ActorsToFind.Num() > 0)
		{
			APathGenerator* FoundPathGenerator = Cast<APathGenerator>(ActorsToFind[0]);
			FoundPathGenerator->AddFloorTile();

			FTimerHandle TimerHandle;

			//GetWorldTimerManager().SetTimer(TimerHandle, this, &ATimeKollapsed_PlayerController::AddWidgetToViewPort, GameOverDelay, false);
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ATile::DestroyTile, 2.f, false);

		}
	}
}

void ATile::SpawnRandomLocation(UBoxComponent* spawnArea)
{
	// TODO: Check if I have to spawn something
	if (ObstacleArray.Num() > 0)
	{
		FVector vOrigin = spawnArea->Bounds.Origin;
		FVector vBoxExtent = spawnArea->Bounds.BoxExtent;

		//FVector vRandomPoint = RandomPointInBoundingBox(vOrigin, vBoxExtent);
		const FVector BoxMin = vOrigin - vBoxExtent;
		const FVector BoxMax = vOrigin + vBoxExtent;
		FVector vRandomPoint = FMath::RandPointInBox(FBox(BoxMin, BoxMax));

		int iRandomIndex = FMath::RandRange(0, ObstacleArray.Num() - 1);

		AActor* obstacle = GetWorld()->SpawnActor<AActor>(ObstacleArray[iRandomIndex]/*, vRandomPoint, FRotator::ZeroRotator*/);
		//obstacle->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
		obstacle->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		vRandomPoint.Z = GetActorLocation().Z - 12;
		obstacle->SetActorLocation(vRandomPoint);
		obstacle->SetActorRotation(GetActorRotation());
	}
}

void ATile::DestroyTile()
{
	TArray<AActor*> childActors;
	GetAttachedActors(childActors);
	for (auto childActor : childActors)
	{
		childActor->Destroy();
	}
	this->Destroy();
}

