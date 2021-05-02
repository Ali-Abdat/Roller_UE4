// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coins.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCoinsTaken);

UCLASS(hidecategories = ("Dispatcher"))
class ROLLER_API ACoins : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoins();
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, Category = "Dispatcher")
		FCoinsTaken CoinTaken;
	//Coin ScorePoint
	UPROPERTY(EditAnywhere, Category = "Coins")
		int ScorePoint;
private:

	UPROPERTY()
		class UStaticMeshComponent * CoinMesh;

	UPROPERTY()
		class USceneComponent * SceneDefult;
	// helps the Coin to rotates
	UPROPERTY()
		class URotatingMovementComponent * RotationMovement;
	// Adding Dynamic Trigger for OnComponentoverlap with the Static mesh
	UFUNCTION()
		void TriggerEnter(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
