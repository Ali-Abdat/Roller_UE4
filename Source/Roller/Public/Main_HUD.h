// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Main_HUD.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScore,int,ScoreTXT);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthPercentage, float, HealthPercentagePB);

UCLASS()
class ROLLER_API UMain_HUD : public UUserWidget
{
	GENERATED_BODY()

		virtual bool Initialize() override;
public:
	//After Adding the Text block in the Designer giving the Exact name and use the meta bindWidget to be able use it as ref
		UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
		 class UTextBlock* Score_TXT;
		//After Adding the ProgressBar in the Designer giving the Exact name and use the meta bindWidget to be able use it as ref
		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Roller_HP_PB;
		
		UPROPERTY(BlueprintAssignable, Category = "DisPatcher")
		FScore ScoreDP;
		UPROPERTY(BlueprintAssignable, Category = "DisPatcher")
		FHealthPercentage HealthPercentageDP;
		UPROPERTY(BlueprintReadWrite, Category = "Score")
			int Score;
		UPROPERTY(BlueprintReadWrite, Category = "Score")
			TSubclassOf<class UGameOver_HUD> GameOverWidget;
		//Added Dynamic for HealthPercentageDP Dispatcher to change the Percentage and Color of the HealthBar in HUD
		UFUNCTION()
			void ProgressEffect(float Prog_Effect);
		//Added Dynamic for ScoreDP Dispatcher to change the Score in HUD
		UFUNCTION()
			void ScoreChanger(int ScoreChanger);
		
};