// Copyright Jorge Kuijper. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Obstacle.generated.h"

UCLASS()
class ENDLESSZOMBIE_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	UPROPERTY(EditAnywhere, Category = "Obstacle-Components")
		USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, Category = "Obstacle-Components")
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category = "Obstacle-Components")
		UBoxComponent* ObstacleCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bIsCollisioned = false;

};
