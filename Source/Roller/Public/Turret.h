// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class ARollerBall;
UCLASS()
class ROLLER_API ATurret : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

	UFUNCTION(Category = "Turret")
		void SetupTurret(float RotationSpeed,float LowerRotation, float HigherRotation , float BulletSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "Scene")
		USceneComponent* SceneDefault;
	UPROPERTY(BlueprintReadWrite, Category = "Scene")
		UStaticMeshComponent* Body;
	UPROPERTY(BlueprintReadWrite, Category = "Scene")
		USceneComponent* SceneForRotation;
	UPROPERTY(BlueprintReadWrite, Category = "Scene")
		UStaticMeshComponent* Weapon;
	UPROPERTY(BlueprintReadWrite, Category = "Scene")
		USceneComponent* Muzzle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		bool TargetSeen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		bool Switcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float CurrentAngle;;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float LargerYawRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float SmallerYawRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		class ARollerBall* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float ProjectileSpeed;
private:
	UPROPERTY()
		FTimerHandle ProjectileTimer;


private:
	// Trace for roller Movement
	UFUNCTION(Category = "Turret")
		void TraceForMovingPawn();
	// Spawning Projectile
	UFUNCTION(Category = "Turret")
	void ProjectileSpawner();
	//Find Player and return the Rotation and predicting his next movement location
	UFUNCTION(Category = "Turret")
	FRotator PredictPlayerLocation(FVector SceneLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};