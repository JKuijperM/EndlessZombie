// Copyright Jorge Kuijper. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "Brain.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSZOMBIE_API ABrain : public APickupItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABrain();

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
