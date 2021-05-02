// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_rocket.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectile_rocket::AProjectile_rocket()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RocketMesh(TEXT("/Game/SciFiWeapDark/Weapons/Darkness_RocketLauncher_Ammo")); 
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile_rocket::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	Rocket_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	Rocket_Mesh->SetStaticMesh(RocketMesh.Object);
	Rocket_Mesh->SetupAttachment(CollisionComp);
	Rocket_Mesh->SetRelativeLocation(FVector(-15.0f, 0, 0));


	// Die after 3 seconds by default
	InitialLifeSpan = 5.0f;

}

void AProjectile_rocket::SetupBullet(float BulletSpeed)
{
	ProjectileMovement->InitialSpeed = BulletSpeed;
	ProjectileMovement->MaxSpeed = BulletSpeed;
}

void AProjectile_rocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, 30, GetVelocity().GetSafeNormal(), Hit, GetInstigatorController(), this, UDamageType::StaticClass());
		Destroy();
	}
}