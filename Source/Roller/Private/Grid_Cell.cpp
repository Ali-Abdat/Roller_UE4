// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid_Cell.h"
#include "MyLibraryHelper.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Roller/RollerBall.h"
#include "DrawDebugHelpers.h"
#include "Coins.h"
#include "Turret.h"
//Sets default values
AGrid_Cell::AGrid_Cell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneDefault = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneDefault;
	DeathZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Death"));
	DeathZone->AttachTo(RootComponent);
	DeathZone->SetRelativeLocation(FVector(0.0f, 0.0f, -100.f));
	DeathZone->OnComponentBeginOverlap.AddDynamic(this, &AGrid_Cell::OnOverlapBegin);
	// Getting the Mesh from the Content.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> floorMesh(TEXT("/Game/FreeMarket_Grid/Source/Meshes/SuperGrid_BoxCentered"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMesh(TEXT("/Game/FreeMarket_Grid/Source/Meshes/SuperGrid_CylinderFull"));
	Wall_Mesh = WallMesh.Object;
	Floor_Mesh = floorMesh.Object;

	Wall_Thickness = 2.0f;
	MaxNumOfCoins = 20;
	ProjectileSpeed = 1000.f;
	TurretSize = 50;
	CurrentNumOfCoins = 0;
}

// Update in Editor Time the Length and Width Vector to show the How many rows and Columns.
void AGrid_Cell::OnConstruction(const FTransform& Transform)
{
	Length_ = Length_ * FVector(1.0f, 0, 0);
	Width_ = Width_ * FVector(0, 1.0f, 0);
	BuildingGrid();
	DeathZone->SetRelativeLocation((Length_ /2)+ (Width_/2)+ (FVector(0.0f, 0.0f, -300.f)));
	DeathZone->SetBoxExtent(Length_  + Width_ + (FVector(0.0f, 0.0f, 100.f)));
	SetGridCellColumn_Row();
}

// Called when the game starts or when spawned
void AGrid_Cell::BeginPlay()
{
	Super::BeginPlay();
	TimeSpawner();
	BuildingGrid();
	int BallPosition = FMath::RandRange(0, NumberOfCells);
	SetBallPosition(BallPosition);
	SettingTurretsAutomatic();
	for (FTurretStruct TurretSetting : TurretSettings)
	{
		SpawnTurrets(TurretSetting.CellNumber, TurretSetting.RotationSpeed, TurretSetting.DirectionLook, TurretSetting.LowerRotation, TurretSetting.HigherRotation, TurretSetting.BulletSpeed);
	}
}

void AGrid_Cell::SetGridCellColumn_Row()
{
	float Wall = Wall_Thickness * 100.0f;
	Columns = (int)((Width_.Y - Wall) / 200);
	Rows = (int)((Length_.X - Wall) / 200);
	NumberOfCells = (Rows * Columns-1);
}

FVector AGrid_Cell::GetWallLengthSize()
{
	FVector size;
	size.X = Wall_Thickness;
	size.Y = Wall_Thickness;
	size.Z = (Length_.X / 100.0f);
	return size;
}

FVector AGrid_Cell::GetWallWidthSize()
{
	FVector size;
	size.X = Wall_Thickness;
	size.Y = Wall_Thickness;
	size.Z = (Width_.Y / 100.0f);
	return size;
}

FVector AGrid_Cell::BestLocationForCoins()
{
	FVector RandomLocation;
	FVector ActorLocation = GetActorLocation();
	float AvoidWall = (Wall_Thickness * 60);
	//Get The X
	float locationX = Length_.X - AvoidWall;
	float MinMaxLocationX = FMath::RandRange(AvoidWall, locationX);
	RandomLocation.X = ActorLocation.X + MinMaxLocationX;
	//Get The Y
	float locationY = Width_.Y - AvoidWall;
	float MinMaxLocationY = FMath::RandRange(AvoidWall, locationY);
	RandomLocation.Y = ActorLocation.Y + MinMaxLocationY;
	//Get The Z
	RandomLocation.Z = ActorLocation.Z + 5.0f;
	return RandomLocation;
}

FVector AGrid_Cell::GetFloorSize()
{
	FVector FloorSize = FVector(ForceInitToZero);

	FloorSize.X = (Width_.Y / 100) - Wall_Thickness;
	//TobeLikePlane
	FloorSize.Y = 0.1f;

	FloorSize.Z = (Length_.X / 100.0f) - Wall_Thickness;
	return FloorSize;
}

FVector AGrid_Cell::GetFloorOffset()
{
	FVector FloorOffset = FVector(ForceInitToZero);
	FloorOffset.Y = (Width_.Y / 2.0f) + Wall_Thickness;
	FloorOffset.X = Wall_Thickness * 50.0f;
	return FloorOffset;
}

void AGrid_Cell::CoinSpawner()
{
	FVector CoinLocation = BestLocationForCoins();

	//For Optimizing and Make it look Better I want to Test the Box if going to hit another coin to avoid overlapping
	FVector Start = CoinLocation + FVector(0.0f, 0.0f, 1.0f);
	FVector End = CoinLocation - FVector(0.0f, 0.0f, 1.0f);
	ECollisionChannel CollisionChannel = ECC_WorldDynamic;
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(200, 200, 200));
	FCollisionQueryParams CollisionQuery(FName(TEXT("Trace")), false, this);
	FHitResult Hit;
	bool GoingtoHit = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, CollisionChannel, Shape, CollisionQuery);
	if (!GoingtoHit)
	{

		ACoins * CoinsRef = GetWorld()->SpawnActor<ACoins>(CoinLocation, FRotator(ForceInitToZero));
		CurrentNumOfCoins++;
		CoinsRef->CoinTaken.AddDynamic(this, &AGrid_Cell::DecrementCoin);
	}
}

void AGrid_Cell::DecrementCoin()
{
	CurrentNumOfCoins--;
	if (!(ReSpawnCoinTimer.IsValid()))
	{
		GetWorld()->GetTimerManager().SetTimer(ReSpawnCoinTimer, this, &AGrid_Cell::TimeSpawner, 3, false);
	}
}

void AGrid_Cell::TimeSpawner()
{
	GetWorld()->GetTimerManager().SetTimer(CoinCheckerTimer, this, &AGrid_Cell::CoinChecker, 0.1, true);
}

void AGrid_Cell::CoinChecker()
{
	if (CurrentNumOfCoins == MaxNumOfCoins)
	{
		GetWorld()->GetTimerManager().ClearTimer(CoinCheckerTimer);
		GetWorld()->GetTimerManager().ClearTimer(ReSpawnCoinTimer);
	}
	else
	{
		CoinSpawner();
	}
}

void AGrid_Cell::SetBallPosition(int CellNumber)
{
	FVector Location = CellLocation(CellNumber);
	ARollerBall* Player = Cast<ARollerBall>(UGameplayStatics::GetPlayerPawn(this, 0));
	Location.Z += 60;
	Player->SetActorLocation(Location);
}

void AGrid_Cell::SettingTurretsAutomatic()
{
	TurretSettings.Empty();
	TArray<FString> Direc = { "N", "S", "E", "W" };
	int TempDirec;
	TArray<int32> TempCells;
	if (NumberOfCells < TurretSize)
		TurretSize = FMath::RandRange(0, NumberOfCells - 1);
	for (int i = 0; i < TurretSize; i++)
	{
		FTurretStruct TempTurretSetting;
		TempTurretSetting.CellNumber = FMath::RandRange(0, NumberOfCells);
		//Check if the Cell number already contain a turret
		if (TempCells.Contains(TempTurretSetting.CellNumber))
		{
			i--;
		}
		else
		{
			TempCells.Add(TempTurretSetting.CellNumber);
			TempTurretSetting.BulletSpeed = ProjectileSpeed;
			TempDirec = FMath::RandRange(0, 3);
			TempTurretSetting.DirectionLook = Direc[TempDirec];
			TempTurretSetting.LowerRotation = FMath::RandRange(-180, 0);
			TempTurretSetting.HigherRotation = FMath::RandRange(0, 180);
			TempTurretSetting.RotationSpeed = FMath::RandRange(0.1f, TurretRotationSpeed);
			TurretSettings.Add(TempTurretSetting);
		}
	}
}

void AGrid_Cell::SpawnTurrets(int CellNumber, float RotationSpeed, FString DirectionLook, float LowerRotation, float HigherRotation , float BulletSpeed)
{
	FVector Location = CellLocation(CellNumber);
	FRotator Turret_Rotation;

	#pragma region RotationofTurret
	if (DirectionLook == "N")
	{
		Turret_Rotation = FRotator(0.0f, 90.0f, 0);
	}
	else if (DirectionLook == "E")
	{
		Turret_Rotation = FRotator(0.0f, 180.0f, 0);
	}
	else if (DirectionLook == "S")
	{
		Turret_Rotation = FRotator(0.0f, 270.0f, 0);
	}
	else if (DirectionLook == "W")
	{
		Turret_Rotation = FRotator(0.0f, 0.0f, 0);
	}
	else
	{
		Turret_Rotation = FRotator(0.0f, 0.0f, 0);
	}
	#pragma endregion

	FTransform SpawnTransform(Turret_Rotation, Location);
	ATurret * TurretRef = GetWorld()->SpawnActorDeferred<ATurret>(ATurret::StaticClass(), SpawnTransform,this,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	TurretRef->SetupTurret(RotationSpeed, LowerRotation, HigherRotation , BulletSpeed);
	TurretRef->FinishSpawning(SpawnTransform);
}

FVector AGrid_Cell::CellLocation(int CellNumber)
{
	int RowNumber = CellNumber / Columns;
	int ColNumber = FMath::Max(0, (CellNumber % Columns));
	float Additon = (Wall_Thickness * 100.0f);
	FVector SelfLocation = GetActorLocation();
	FVector Location;
	Location.X = (RowNumber * 200) + Additon + SelfLocation.X;
	Location.Y = (ColNumber * 200) + Additon + SelfLocation.Y;
	Location.Z = SelfLocation.Z;
	return Location;
}

FVector AGrid_Cell::Length_Func()
{
	float Additon = Wall_Thickness * 100.0f;
	FVector Location = FVector::ZeroVector;
	Location.X = (Rows * 200) + Additon;
	return Location;
}

FVector AGrid_Cell::Width_Func()
{
	float Additon = Wall_Thickness * 100.0f;
	FVector Location = FVector::ZeroVector;
	Location.Y = (Columns * 200) + Additon;
	return Location;
}

void AGrid_Cell::OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("dsafa"));
	ARollerBall* Player = Cast<ARollerBall>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (OtherActor == Player)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, 3000000000000, GetVelocity().GetSafeNormal(), SweepResult, GetInstigatorController(), this, UDamageType::StaticClass());
	}
}

void AGrid_Cell::CreateSections(FVector SectionSize, FVector SectionLocation, UStaticMesh *SectionMesh, bool Orientation)
{
	FRotator Rotator = (Orientation) ? FRotator(0.0, 90.0f, -90.0f) : FRotator(0.0, 0.0f, 90.0f);

	UStaticMeshComponent* NewComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), NAME_None, RF_Transient);
	check(NewComponent);
	NewComponent->RegisterComponent();
	NewComponent->SetStaticMesh(SectionMesh);
	NewComponent->SetRelativeTransform(FTransform(Rotator, SectionLocation + GetActorLocation(), SectionSize));
	RegisteredComponent.Add(NewComponent);
}

void AGrid_Cell::BuildingGrid()
{
	for (UStaticMeshComponent* Comp : RegisteredComponent)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}
	RegisteredComponent.Empty();
	FVector Zero = FVector(ForceInitToZero);
	//Blocking Wall
	CreateSections(GetWallLengthSize(), Zero, Wall_Mesh, true);
	CreateSections(GetWallWidthSize(), Zero, Wall_Mesh, false);
	CreateSections(GetWallWidthSize(), Length_, Wall_Mesh, false);
	CreateSections(GetWallLengthSize(), Width_, Wall_Mesh, true);
	//Floor
	CreateSections(GetFloorSize(), GetFloorOffset(), Floor_Mesh, true);
}