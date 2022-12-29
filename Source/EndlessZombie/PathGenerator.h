// Copyright Jorge Kuijper. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "PathGenerator.generated.h"

UCLASS()
class ENDLESSZOMBIE_API APathGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Tiles")
		TSubclassOf<class ATile> Tile;

	UFUNCTION(Category = "Tiles")
		void AddFloorTile();

	FTransform tNextSpawnPoint = FTransform(FVector(90.0f, 1110.0f, 0.0f));
};
