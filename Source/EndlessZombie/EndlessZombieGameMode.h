// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessZombieGameMode.generated.h"

UCLASS(minimalapi)
class AEndlessZombieGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEndlessZombieGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void RestartLevel();

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<class UZombiePlayerHUD> ZombiePlayerHUDClass;
	UPROPERTY()
		class UZombiePlayerHUD* ZombieHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
		int iPlayerLive = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		int iScore = 0;

	void UpdateLive();
	void UpdateScore(int iNewScore);
};



