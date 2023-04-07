// Copyright Jorge Kuijper. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZombiePlayerHUD.generated.h"

class UTextBlock;
class UImage;
class AEndlessZombieGameMode;
/**
 *
 */
UCLASS()
class ENDLESSZOMBIE_API UZombiePlayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TextLife;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* LifeImg01;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* LifeImg02;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* LifeImg03;
	UPROPERTY(EditAnywhere)
		UTexture2D* LifeGreenTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* LifeGreyTexture;

	AEndlessZombieGameMode* CurrentGameMode;

	void ModifyLifeCounter();

private:
	void EmptyLife(UImage* LifeImg);
};
