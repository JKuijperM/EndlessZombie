// Copyright Jorge Kuijper. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "TileCurve.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSZOMBIE_API ATileCurve : public ATile
{
	GENERATED_BODY()

public:
	ATileCurve();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* TurnTrigger;

	UPROPERTY(EditAnywhere, Category = "Turn")
		bool bTurnRight = true;
	UPROPERTY(EditAnywhere, Category = "Turn")
		bool bTurnLeft = false;

	UFUNCTION()
		virtual void TurnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

private:
	bool bWasTriggered = false;
};
