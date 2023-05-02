// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessZombieGameMode.h"
#include "ZombiePlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "EndlessZombieCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AEndlessZombieGameMode::AEndlessZombieGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/OLD/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// HUD
	ZombiePlayerHUDClass = nullptr;
	ZombieHUD = nullptr;
}

void AEndlessZombieGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ZombiePlayerHUDClass)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ZombieHUD = CreateWidget<UZombiePlayerHUD>(PlayerController, ZombiePlayerHUDClass);
		if (ZombieHUD)
			ZombieHUD->AddToPlayerScreen();
	}
}

void AEndlessZombieGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ZombieHUD)
	{
		ZombieHUD->RemoveFromParent();
		ZombieHUD = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AEndlessZombieGameMode::RestartLevel()
{
	FName LevelName = FName(GetWorld()->GetCurrentLevel()->GetName());
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void AEndlessZombieGameMode::UpdateLive()
{
	iPlayerLive--;
	ZombieHUD->UpdateLiveCounter();
}

void AEndlessZombieGameMode::UpdateScore(int iNewScore)
{
	iScore += iNewScore;
	UE_LOG(LogTemp, Warning, TEXT("[AEndlessZombieGameMode::UpdateScore] %d"), iScore);
	ZombieHUD->UpdateScoreCounter(iScore);
}
