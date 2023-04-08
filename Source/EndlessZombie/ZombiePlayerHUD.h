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
		UTextBlock* TextLive;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* LiveImg01;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* LiveImg02;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* LiveImg03;
	UPROPERTY(EditAnywhere)
		UTexture2D* LiveGreenTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* LiveGreyTexture;

	AEndlessZombieGameMode* CurrentGameMode;

	void ModifyLiveCounter();

private:
	void EmptyLive(UImage* LiveImg);
};
