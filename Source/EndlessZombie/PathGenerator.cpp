// Copyright Jorge Kuijper. All Rights Reserved.


#include "PathGenerator.h"

// Sets default values
APathGenerator::APathGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 10; i++)
	{
		AddFloorTile();
	}
	
}

// Called every frame
void APathGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APathGenerator::AddFloorTile()
{
	if (Tile != nullptr)
	{
		ATile* GeneratedTile = GetWorld()->SpawnActor<ATile>(Tile, tNextSpawnPoint);
		tNextSpawnPoint = GeneratedTile->GetAttachTransform();
	}	
}

