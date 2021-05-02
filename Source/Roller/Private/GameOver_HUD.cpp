// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOver_HUD.h"
#include "kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UGameOver_HUD::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	Retry_BTN->OnPressed.AddDynamic(this, &UGameOver_HUD::TryAgain);
	Exit_BTN_1->OnPressed.AddDynamic(this, &UGameOver_HUD::Quit);
	return true;
}

void UGameOver_HUD::NativeConstruct()
{
	Super::Construct();
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = 1;
	PlayAnimation(GameOverText);
	FText ScoreText = FText::FromString(FString::FromInt(HighScore));
	HighScore_TXT->SetText(ScoreText);

}

void UGameOver_HUD::TryAgain()
{
	UGameplayStatics::OpenLevel(this, "RollingExampleMap1");
}

void UGameOver_HUD::Quit()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}

void UGameOver_HUD::SetterHighScore(int HS)
{
	HighScore = HS;
}
