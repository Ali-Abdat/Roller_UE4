// Copyright Epic Games, Inc. All Rights Reserved.

#include "RollerGameMode.h"
#include "RollerBall.h"
#include "Kismet/GameplayStatics.h"

void ARollerGameMode::BeginPlay()
{
	SetRollerBall(Cast<ARollerBall>(UGameplayStatics::GetPlayerPawn(this, 0)));
}

ARollerGameMode::ARollerGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ARollerBall::StaticClass();
}

void ARollerGameMode::SetRollerBall(ARollerBall* newBall)
{
	RefRollerBall = newBall;
}

ARollerBall* ARollerGameMode::GetRollerBall()
{
	return RefRollerBall;
}
