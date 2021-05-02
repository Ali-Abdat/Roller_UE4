// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Roller/RollerBall.h"
#include "Projectile_rocket.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATurret::ATurret()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Body_Mesh(TEXT("/Game/Geometry/Body"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Weapon_Mesh(TEXT("/Game/Geometry/Weapon"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	SceneDefault = CreateDefaultSubobject<USceneComponent>(TEXT("SceneDefault"));
	SceneForRotation = CreateDefaultSubobject<USceneComponent>(TEXT("SceneForRotation"));
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	//Root
	RootComponent = SceneDefault;

	//Constructing Body
	Body->SetStaticMesh(Body_Mesh.Object);
	Body->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	Body->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.5f));
	Body->SetupAttachment(SceneDefault);
	//Constructing SceneRotation
	SceneForRotation->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	SceneForRotation->SetupAttachment(SceneDefault);
	//Constructing Weapon
	Weapon->SetStaticMesh(Weapon_Mesh.Object);
	Weapon->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	Weapon->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	Weapon->SetRelativeScale3D(FVector(2.0f, 2.0f, 1.5f));
	Weapon->SetupAttachment(SceneForRotation);
	//Constructing MUZZLE
	Muzzle->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	Muzzle->SetRelativeRotation(FRotator(70.0f, 90.0f, 90.0f));
	Muzzle->SetupAttachment(Weapon);
	//Scene rotation
	SceneForRotation->SetRelativeRotation(FRotator(ForceInitToZero));

	//Starting Vars
	CurrentAngle = 0;
	Switcher = false;
	ProjectileSpeed = 3000.0f;
}

void ATurret::SetupTurret(float RotationSpeed, float LowerRotation, float HigherRotation, float BulletSpeed)
{
	Speed = RotationSpeed;
	SmallerYawRotation = LowerRotation;
	LargerYawRotation = HigherRotation;
	ProjectileSpeed = BulletSpeed;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ARollerBall>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATurret::TraceForMovingPawn()
{
	FVector Start = Muzzle->GetComponentLocation();
	FVector End = Start + (Muzzle->GetForwardVector() * 600.0f);
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObject;
	TArray<AActor*> ACtors;
	CollisionObject.Add(EObjectTypeQuery::ObjectTypeQuery4);
	FHitResult Hit;
	bool GoingtoHit = UKismetSystemLibrary::CapsuleTraceSingleForObjects(this, Start, End, 100.0f, 600.0f, CollisionObject, false, ACtors, EDrawDebugTrace::None, Hit, true);
	DrawDebugLine(GetWorld(), Start, End, FColor::Emerald, false, 0, 0, 3);
	if (GoingtoHit)
	{
		TargetSeen = true;
		if (!(ProjectileTimer.IsValid()))
		{
			GetWorld()->GetTimerManager().SetTimer(ProjectileTimer,this, &ATurret::ProjectileSpawner, 3.0, true);
		}
	}
	else
	{
		TargetSeen = false;
		GetWorld()->GetTimerManager().ClearTimer(ProjectileTimer);
	}
}

void ATurret::ProjectileSpawner()
{
	FVector Muzzle_Location = Muzzle->GetComponentLocation();
	FRotator Projectile_Rotation = PredictPlayerLocation(Muzzle_Location);
	FTransform SpawnTransform(Projectile_Rotation, Muzzle_Location);
	AProjectile_rocket * Bulllet = GetWorld()->SpawnActorDeferred<AProjectile_rocket>(AProjectile_rocket::StaticClass(), SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Bulllet->SetupBullet(ProjectileSpeed);
	Bulllet->FinishSpawning(SpawnTransform);
}

FRotator ATurret::PredictPlayerLocation(FVector SceneLocation)
{
	FVector Player_Location = Player->GetActorLocation();
	FVector Distance_travel = Player_Location - GetActorLocation();
	float TimeTaketoReach = Distance_travel.Size() / ProjectileSpeed;
	FVector PrecitedLocation = Player_Location + (Player->GetVelocity()*TimeTaketoReach);
	FRotator Projectile_Rotation = UKismetMathLibrary::FindLookAtRotation(SceneLocation, PrecitedLocation);
	return Projectile_Rotation;
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForMovingPawn();
	if (!TargetSeen)
	{
		float SceneRotationYaw = SceneForRotation->GetComponentRotation().Yaw;
		float target = (Switcher) ? SmallerYawRotation : LargerYawRotation  ;
		float NewYaw = UKismetMathLibrary::FInterpTo_Constant(SceneRotationYaw, target, DeltaTime, Speed);
		FRotator NewRotation = FRotator(0.0f, NewYaw, 0.0f);
		SceneForRotation->SetWorldRotation(NewRotation);
		float NewTarget = SceneRotationYaw + ((Switcher) ? -1.0f : 1.0f);
		if (CurrentAngle >= NewTarget)
		{
			CurrentAngle = LargerYawRotation;
			Switcher = false;
		}
		else
		{
			CurrentAngle = SmallerYawRotation;
			Switcher = true;
		}
	}
	else
	{
		FVector Muzzle_Location = Muzzle->GetComponentLocation();
		FRotator LookingAtPlayerRotation = PredictPlayerLocation(Muzzle_Location);
		LookingAtPlayerRotation.Roll = 0.0f;
		LookingAtPlayerRotation.Pitch = 0.0f;
		LookingAtPlayerRotation -= FRotator(0.0f, 90.0f, 0.0f);
		SceneForRotation->SetWorldRotation(LookingAtPlayerRotation);
	}
}

