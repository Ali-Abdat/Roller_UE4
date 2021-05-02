// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid_Cell.generated.h"

class UMaterialInterface;
class UStaticMesh;
class UStaticMeshComponent;
class USceneComponent;

USTRUCT(BlueprintType)
struct FTurretStruct
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	int CellNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	FString DirectionLook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float LowerRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float HigherRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float BulletSpeed;
};

UCLASS(hidecategories = ("Input", "LOD", "HLOD","Collision","Rendering","Mobile","Cooking","Actor","Assest User Data","Navigation"))
class ROLLER_API AGrid_Cell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid_Cell();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", Meta = (MakeEditWidget = true))
		FVector Length_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", Meta = (MakeEditWidget = true))
		FVector Width_;


	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DeathZone)
		class UBoxComponent* DeathZone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Wall" ,meta = (ClampMin = "2", ClampMax = "4", UIMin = "2", UIMax = "4"))
		float Wall_Thickness;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Wall")
		UStaticMesh* Wall_Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Floor")
		UStaticMesh* Floor_Mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Grid")
		int Rows;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Grid")
		int Columns;
	//Count Start from 0
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Grid")
		int NumberOfCells;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Coins")
		int MaxNumOfCoins;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = "Settings|Coins")
		int CurrentNumOfCoins;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Turret", meta = (ClampMin = "1000", ClampMax = "30000", UIMin = "1000", UIMax = "30000"))
		float ProjectileSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Turret", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
		float TurretRotationSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Turret")
		int TurretSize;
private:
	UPROPERTY()
		FTimerHandle CoinCheckerTimer;
	UPROPERTY()
		FTimerHandle ReSpawnCoinTimer;
	UPROPERTY()
		USceneComponent* SceneDefault;
	UPROPERTY()
		TArray<UStaticMeshComponent*> RegisteredComponent;
	UPROPERTY()
		TArray<FTurretStruct> TurretSettings;
	// Setting Column and Rows Through the onConstruction only.
	UFUNCTION(Category = "Grid")
		void SetGridCellColumn_Row();
	// Getting the Length of the wall
	UFUNCTION(BlueprintPure , Category = "Grid")
		FVector GetWallLengthSize();
	// Getting the Scale of the wall
	UFUNCTION(BlueprintPure, Category = "Grid")
		FVector GetWallWidthSize();
	// Finding the Best Location for Coins to Spawn
	UFUNCTION(BlueprintPure, Category = "Grid")
		FVector BestLocationForCoins();
	// Getting Scale of the floor
	UFUNCTION(BlueprintPure, Category = "Grid")
		FVector GetFloorSize();
	// Getting the Offset of the Floor to not overlap with the walls
	UFUNCTION(BlueprintPure, Category = "Grid")
		FVector GetFloorOffset();
	// Adding Static Mesh to Create the wall and the floor
	UFUNCTION(BlueprintCallable, Category = "Grid")
		void CreateSections(
			FVector SectionSize,
			FVector SectionLocation,
			UStaticMesh *SectionMesh,
			bool Orientation
		);
	// Start the Process by using the CreateSections Func
	UFUNCTION(BlueprintCallable, Category = "Grid")
		void BuildingGrid();
	// Spawn Coins
	UFUNCTION(BlueprintCallable, Category = "Coins")
		void CoinSpawner();
	// Decrementing Coins Count
	UFUNCTION(BlueprintCallable, Category = "Coins")
		void DecrementCoin();
	// TimerSpawner for the coins
	UFUNCTION(BlueprintCallable, Category = "Coins")
		void TimeSpawner();
	// Check if the Coins Reach to its max number
	UFUNCTION(BlueprintCallable, Category = "Coins")
		void CoinChecker();
	// Setting Ball on a Cell Number
	UFUNCTION(BlueprintCallable, Category = "Coins")
		void SetBallPosition(int CellNumber);
	//It's CallInEditor to Set the turretSettingArrayAutomatic
	UFUNCTION(CallInEditor, Category = "Grid")
		void SettingTurretsAutomatic();
	// Spawning Turrets
	UFUNCTION(BlueprintCallable, Category = "Turret")
		void SpawnTurrets(
			int CellNumber,float RotationSpeed,
			FString DirectionLook, float LowerRotation,
			float HigherRotation, float BulletSpeed
		);

	// Finding the Location of the Chosen Cell
	UFUNCTION(BlueprintCallable, Category = "Helper")
		FVector CellLocation(int CellNumber);
	// Finding Length Vector from the Columns
	UFUNCTION(BlueprintCallable, Category = "Helper")
		FVector Length_Func();
	//Find Width Vector from the Rows
	UFUNCTION(BlueprintCallable, Category = "Helper")
		FVector Width_Func();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult & SweepResult);
};