// Copyright Jorge Kuijper. All Rights Reserved.


#include "Brain.h"
#include "ZombieCharacter.h"
#include "EndlessZombieGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

ABrain::ABrain()
{
	// Set the brain mesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/StaticMeshes/Brain/SM_Brain.SM_Brain"));
	if (MeshObj.Succeeded())
		StaticMesh->SetStaticMesh(MeshObj.Object);

	CollisionTrigger->SetRelativeLocation(FVector(0.f, -3.f, 2.f));
	CollisionTrigger->SetRelativeScale3D(FVector(0.5f, 0.61f, 0.52f));
	iPunctuation = 11;
}

void ABrain::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OtherActor);
	if (ZombieCharacter)
	{
		AEndlessZombieGameMode* CurrentGameMode = Cast<AEndlessZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (CurrentGameMode)
			CurrentGameMode->UpdateScore(iPunctuation);
	}
}
