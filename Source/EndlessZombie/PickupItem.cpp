// Copyright Jorge Kuijper. All Rights Reserved.


#include "PickupItem.h"
#include "ZombieCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
APickupItem::APickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	CollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionTrigger"));
	CollisionTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnBeginOverlap);

}

void APickupItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OtherActor);
	if (ZombieCharacter)
		this->Destroy();
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

