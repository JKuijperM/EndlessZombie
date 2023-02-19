// Copyright Jorge Kuijper. All Rights Reserved.


#include "TileCurve.h"
#include "ZombieCharacter.h"

ATileCurve::ATileCurve()
{
	TurnTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnTrigger"));
	TurnTrigger->SetupAttachment(SceneComponent);
	TurnTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}


void ATileCurve::BeginPlay()
{
	Super::BeginPlay();

	TurnTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATileCurve::TurnBeginOverlap);

}


void ATileCurve::TurnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OtherActor);

	if (ZombieCharacter != nullptr)
	{
		if (!bWasTriggered)
		{
			if (bTurnRight && !bTurnLeft)
			{
				ZombieCharacter->bStraight = false;
				ZombieCharacter->iSideTurn = 1;
			}
			else if (!bTurnRight && bTurnLeft)
			{
				ZombieCharacter->bStraight = false;
				ZombieCharacter->iSideTurn = -1;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[ATileCurve::OnBeginOverlapChild] You should not be here, check if bTurnRight and bTurnLeft have the same value."))
			}
			bWasTriggered = true;
		}
	}
}