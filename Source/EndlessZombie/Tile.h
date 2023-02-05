// Copyright Jorge Kuijper. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Obstacle.h"
#include "Tile.generated.h"

UCLASS()
class ENDLESSZOMBIE_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UArrowComponent* ArrowComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* EndTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* ObstacleZone;

	UFUNCTION(BlueprintCallable, Category = "AttachedPoint")
		FTransform GetAttachTransform();
	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnRandomLocation(UBoxComponent* boxTrigger);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacles")
		TArray<TSubclassOf<class AObstacle>> ObstacleArray;

private:
	void DestroyTile();
};
