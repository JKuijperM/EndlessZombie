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
		ZombieCharacter->bCanTurn = true;		
	}
}