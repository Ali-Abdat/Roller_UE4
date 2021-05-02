// Copyright Epic Games, Inc. All Rights Reserved.

#include "RollerBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Main_HUD.h"
#include "GameOver_HUD.h"
#include "Kismet/GameplayStatics.h"

ARollerBall::ARollerBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuHUD(TEXT("/Game/UI/MainHud"));
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverHUD(TEXT("/Game/UI/RetryAgain"));
	MenuHUDClass = MenuHUD.Class;
	GameOverHUDClass = GameOverHUD.Class;
	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true); // Rotation of the ball should not affect rotation of boom
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up forces
	RollTorque = 50000000.0f;

	//Set Health
	MaxHealth = 100;
	CurrentHealth = MaxHealth;

	DeathDP.AddDynamic(this, &ARollerBall::OnDeath);
}


void ARollerBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ARollerBall::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARollerBall::MoveForward);
	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &ARollerBall::QuitGame);
}

void ARollerBall::BeginPlay()
{
	Super::BeginPlay();
	MENU = CreateWidget<UMain_HUD>(this->GetGameInstance(), MenuHUDClass);
	MENU->AddToViewport();
}

void ARollerBall::MoveRight(float Val)
{
	const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
	Ball->AddTorqueInRadians(Torque);
}

void ARollerBall::MoveForward(float Val)
{
	const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
	Ball->AddTorqueInRadians(Torque);	
}

void ARollerBall::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}

float ARollerBall::HealthPercentage()
{
	return CurrentHealth / MaxHealth;
}

void ARollerBall::SettingHealth(float newHealth)
{
	CurrentHealth = newHealth;
}

float ARollerBall::GettingHealth()
{
	return CurrentHealth;
}

void ARollerBall::DecrementHealth(float Damage)
{
	SettingHealth(CurrentHealth - Damage);
	MENU->HealthPercentageDP.Broadcast(HealthPercentage());
	if (HealthPercentage()<=0.0f)
	{
		DeathDP.Broadcast();
	}
	
}

void ARollerBall::AddingScore(int newScore)
{
	Score += newScore;
	//binded in Main_HUD class
	MENU->ScoreDP.Broadcast(Score);
}

int ARollerBall::GettingScore()
{
	return Score;
}

float ARollerBall::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	DecrementHealth(DamageAmount);
	return 0.0f;
}

void ARollerBall::OnDeath()
{
	MENU->RemoveFromParent();
	DeathDP.RemoveDynamic(this, &ARollerBall::OnDeath);
	UGameplayStatics::GetPlayerController(this, 0)->UnPossess();
	UGameOver_HUD* RefGameOverMenu = CreateWidget<UGameOver_HUD>(this->GetGameInstance(), GameOverHUDClass);
	RefGameOverMenu->SetterHighScore(Score);
	RefGameOverMenu->AddToViewport();
}
