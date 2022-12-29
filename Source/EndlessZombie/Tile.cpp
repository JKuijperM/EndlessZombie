// Copyright Jorge Kuijper. All Rights Reserved.


#include "Tile.h"
#include "ZombieCharacter.h"
#include "PathGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Add scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SceneComponent->SetupAttachment(RootComponent);
	// Add arrow component like attach point
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	ArrowComponent->SetupAttachment(SceneComponent);

	// Add box collider
	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndPoint"));
	EndTrigger->SetupAttachment(SceneComponent);
	EndTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	EndTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATile::OnBeginOverlap);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ATile::GetAttachTransform()
{
	FTransform tAttachPoint;

	tAttachPoint = ArrowComponent->GetComponentTransform();

	return tAttachPoint;
}

void ATile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OtherActor);

	if (ZombieCharacter != nullptr)
	{
		TArray<AActor*> ActorsToFind;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APathGenerator::StaticClass(), ActorsToFind);
		if (ActorsToFind.Num() > 0)
		{
			APathGenerator* FoundPathGenerator = Cast<APathGenerator>(ActorsToFind[0]);
			FoundPathGenerator->AddFloorTile();

			FTimerHandle TimerHandle;
			
			//GetWorldTimerManager().SetTimer(TimerHandle, this, &ATimeKollapsed_PlayerController::AddWidgetToViewPort, GameOverDelay, false);
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ATile::test, 2.f, false);
			
		}
	}
}

void ATile::DestroyTile()
{
	this->Destroy();
}

