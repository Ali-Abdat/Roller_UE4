// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_HUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameOver_HUD.h"




bool UMain_HUD::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	Roller_HP_PB->SetFillColorAndOpacity(FLinearColor(0.0f, 255.0f, 0.0f));
	Roller_HP_PB->SetPercent(1.0f);
	FText Init_TEXT = FText::AsCultureInvariant("0");
	Score_TXT->SetText(FText(Init_TEXT));
	HealthPercentageDP.AddDynamic(this, &UMain_HUD::ProgressEffect);
	ScoreDP.AddDynamic(this, &UMain_HUD::ScoreChanger);
	return true;
}

void UMain_HUD::ProgressEffect(float Prog_Effect)
{
	Roller_HP_PB->SetPercent(Prog_Effect);
	FLinearColor Green = FLinearColor(0.0f, 255.0f, 0.0f);
	FLinearColor Yellow = FLinearColor(255.0f, 255.0f, 0.0f);
	FLinearColor Red = FLinearColor(255.0f, 0.0f, 0.0f);
	FLinearColor Curr = (Prog_Effect <= 0.5) ? ((Prog_Effect <= 0.2) ? Red : Yellow) : Green;
	Roller_HP_PB->SetFillColorAndOpacity(Curr);
	if (Prog_Effect<=0.0f)
	{
		HealthPercentageDP.RemoveDynamic(this, &UMain_HUD::ProgressEffect);
	}
}

void UMain_HUD::ScoreChanger(int ScoreChanger)
{
	Score = ScoreChanger;
	FText ScoreText = FText::FromString(FString::FromInt(ScoreChanger));
	Score_TXT->SetText(ScoreText);
}
