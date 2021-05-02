// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver_HUD.generated.h"

/**
 * 
 */
UCLASS()
class ROLLER_API UGameOver_HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	//After Adding the Text block in the Designer giving the Exact name and use the meta bindWidget to be able use it as ref
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* HighScore_TXT;
	//After Adding the Button in the Designer giving the Exact name and use the meta bindWidget to be able use it as ref, and the Events
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Retry_BTN;
	// After Adding the Button in the Designer giving the Exact name and use the meta bindWidget to be able use it as ref, and the Events
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Exit_BTN_1;
	// After Adding the Animation in the Designer giving the Exact name and use the meta BindWidgetAnim to be able play the animation
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* GameOverText;
	//Initialize the Designer
	virtual bool Initialize() override;
	//On Constructing the the Menu
	virtual void NativeConstruct() override;
	//Dynamic add for the pressing on the Retry_BTN
	UFUNCTION()
		void TryAgain();
	//Dynamic add for the pressing on the Exit_BTN_1
	UFUNCTION()
		void Quit();
	//Setting the highScore
	UFUNCTION()
		void SetterHighScore(int HS);
private:
	UPROPERTY()
		int HighScore;
};
