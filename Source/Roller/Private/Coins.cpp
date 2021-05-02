// Fill out your copyright notice in the Description page of Project Settings.


#include "Coins.h"
#include "Roller/RollerBall.h"
#include "UObject/ConstructorHelpers.h"
#include "components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ACoins::ACoins()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CoinStaicMesh(TEXT("/Game/Platformer/Meshes/Pl_Coin_01"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneDefult = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RotationMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationMovement"));
	RootComponent = SceneDefult;

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Coin"));
	CoinMesh->SetStaticMesh(CoinStaicMesh.Object);
	CoinMesh->SetupAttachment(SceneDefult);
	CoinMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));
	CoinMesh->SetRelativeScale3D(FVector(1.5f));
	CoinMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	CoinMesh->SetCollisionObjectType(ECC_WorldDynamic);
	CoinMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CoinMesh->OnComponentBeginOverlap.AddDynamic(this, &ACoins::TriggerEnter);

	ScorePoint = 10;

}

void ACoins::TriggerEnter(class UPrimitiveComponent* HitComp, 
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ARollerBall* Player = Cast<ARollerBall>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player == OtherActor)
	{
		//is binded in Grid_Cell class
		CoinTaken.Broadcast();
		Player->AddingScore(ScorePoint);
		Destroy();
	}
}


