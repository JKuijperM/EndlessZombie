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

	for (int i = 0; i < iInitialStraightTiles; i++)
	{
		AddSelectedTile(Tiles[0]);
	}
	AddSelectedTile(Tiles[1]);
	
}

// Called every frame
void APathGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APathGenerator::AddFloorTile()
{
	if (Tiles[0] != nullptr)
	{
		ATile* GeneratedTile = GetWorld()->SpawnActor<ATile>(Tiles[0], tNextSpawnPoint);
		tNextSpawnPoint = GeneratedTile->GetAttachTransform();
	}	
}

void APathGenerator::AddSelectedTile(TSubclassOf<class ATile> Tile)
{
	if (Tile != nullptr)
	{
		ATile* GeneratedTile = GetWorld()->SpawnActor<ATile>(Tile, tNextSpawnPoint);
		tNextSpawnPoint = GeneratedTile->GetAttachTransform();
	}
}

