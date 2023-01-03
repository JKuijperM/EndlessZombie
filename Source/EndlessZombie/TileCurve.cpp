// Copyright Jorge Kuijper. All Rights Reserved.


#include "TileCurve.h"
#include "ZombieCharacter.h"

ATileCurve::ATileCurve()
{
	TurnTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnTrigger"));
	TurnTrigger->SetupAttachment(SceneComponent);
	TurnTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	//TurnTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATileCurve::TurnBeginOverlap);
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
			// Yaw
			float fYawValue;

			if (bTurnRight && !bTurnLeft)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ATileCurve::OnBeginOverlapChild] Rotating right"));
				fYawValue = 90.f;
				FRotator NewRotation = FRotator(0.f, fYawValue, 0.f);
				ZombieCharacter->bStraight = false;
				ZombieCharacter->NewRotation = NewRotation;
				//ZombieCharacter->RotateCharacter(NewRotation);
			}
			else if (!bTurnRight && bTurnLeft)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ATileCurve::OnBeginOverlapChild] Rotating left"));
				fYawValue = -90.f;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[ATileCurve::OnBeginOverlapChild] You should not be here, check if bTurnRight and bTurnLeft have the same value."))
			}
			bWasTriggered = true;
		}
	}
}