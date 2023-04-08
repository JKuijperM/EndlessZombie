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
	if (TextLive)
	{
		TextLive->SetText(FText::FromString(TEXT("Lives: ")));
	}

	if (LiveImg01)
	{
		LiveImg01->SetBrushFromTexture(LiveGreenTexture);
	}

	if (LiveImg02)
	{
		LiveImg02->SetBrushFromTexture(LiveGreenTexture);
	}

	if (LiveImg03)
	{
		LiveImg03->SetBrushFromTexture(LiveGreenTexture);
	}

	CurrentGameMode = Cast<AEndlessZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UZombiePlayerHUD::ModifyLiveCounter()
{
	if (CurrentGameMode)
	{
		switch (CurrentGameMode->iPlayerLife)
		{
		case 2:
			EmptyLive(LiveImg03);
			break;
		case 1:
			EmptyLive(LiveImg02);
			break;
		case 0:
			EmptyLive(LiveImg01);
			break;
		default:
			break;
		}
	}
	
}

void UZombiePlayerHUD::EmptyLive(UImage* LiveImg)
{
	if ((LiveImg) && (LiveGreyTexture))
	{
		LiveImg->SetBrushFromTexture(LiveGreyTexture);
	}
}
