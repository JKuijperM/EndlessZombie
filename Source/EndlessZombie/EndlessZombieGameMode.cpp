// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessZombieGameMode.h"
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
}

void AEndlessZombieGameMode::RestartLevel()
{
	FName LevelName = FName(GetWorld()->GetCurrentLevel()->GetName());
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}
