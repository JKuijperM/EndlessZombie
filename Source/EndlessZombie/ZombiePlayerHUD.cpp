// Copyright Jorge Kuijper. All Rights Reserved.


#include "ZombiePlayerHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "EndlessZombieGameMode.h"
#include "Kismet/GameplayStatics.h"

void UZombiePlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// ItemTitle can be nullptr if we haven't created it in the
	// Blueprint subclass
	if (TextLife)
	{
		TextLife->SetText(FText::FromString(TEXT("Lifes: ")));
	}

	if (LifeImg01)
	{
		LifeImg01->SetBrushFromTexture(LifeGreenTexture);
	}

	CurrentGameMode = Cast<AEndlessZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UZombiePlayerHUD::ModifyLifeCounter()
{
	if (CurrentGameMode)
	{
		switch (CurrentGameMode->iPlayerLife)
		{
		case 2:
			EmptyLife(LifeImg03);
			break;
		case 1:
			EmptyLife(LifeImg02);
			break;
		case 0:
			EmptyLife(LifeImg01);
			break;
		default:
			break;
		}
	}
	
}

void UZombiePlayerHUD::EmptyLife(UImage* LifeImg)
{
	if ((LifeImg) && (LifeGreyTexture))
	{
		LifeImg->SetBrushFromTexture(LifeGreyTexture);
	}
}
