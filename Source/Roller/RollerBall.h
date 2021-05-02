// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollerBall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeath);
UCLASS(config=Game)
class ARollerBall : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	/** Called to move ball forwards and backwards */
	void QuitGame();
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
public:
	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

public:
	ARollerBall();
	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditAnywhere, Category=Ball)
	float RollTorque;


private:
	/** Tells the Health of the roller currently */
	UPROPERTY(EditAnywhere, Category = Ball)
		float CurrentHealth;
	/** Tells the Health of the roller Max */
	UPROPERTY(EditAnywhere, Category = Ball)
		float MaxHealth;
	/** Tells the Current Score */
	UPROPERTY(EditAnywhere, Category = Ball)
		int Score;
	/** HUDClass UI*/
	UPROPERTY(EditAnywhere, Category = Menu)
		TSubclassOf<UUserWidget> MenuHUDClass;
	/** GameOverClass UI */
	UPROPERTY(EditAnywhere, Category = Menu)
		TSubclassOf<UUserWidget> GameOverHUDClass;
	/** HUDRef */
	UPROPERTY(EditAnywhere, Category = Menu)
		class UMain_HUD* MENU;
	/** Delegate */
	UPROPERTY()
		FDeath DeathDP;
public:
	/** Health Percentage*/
	UFUNCTION()
		float HealthPercentage();
	/** Setting Health*/
	UFUNCTION()
		void SettingHealth(float newHealth);
	/** Getting Health*/
	UFUNCTION()
		float GettingHealth();
	// DecrementingHealth and getting the new Health Percentage and Broadcasting it to HUDREF Dispatcher
	UFUNCTION()
		void DecrementHealth(float Damage);
	// Adding to the current Score and Broadcasting it to HUDREF Dispatcher
	UFUNCTION()
		void AddingScore(int newScore);
	// Getting Score
	UFUNCTION()
		int GettingScore();
	// Taking Damage from Turret
	UFUNCTION()
		virtual float TakeDamage
		(
			float DamageAmount,
			struct FDamageEvent const & DamageEvent,
			class AController * EventInstigator,
			AActor * DamageCauser
		);
	// Binded with DeathDP
	UFUNCTION()
		void OnDeath();
};