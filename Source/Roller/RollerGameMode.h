// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RollerGameMode.generated.h"

class ARollerBall;

UCLASS(minimalapi)
class ARollerGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ARollerGameMode();
	/** Setting Roller ball*/
	void SetRollerBall(ARollerBall* newBall);
	/** Getting Roller ball*/
	ARollerBall* GetRollerBall();
private:
	UPROPERTY()
	ARollerBall* RefRollerBall;

};



