// Copyright Jorge Kuijper. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class ENDLESSZOMBIE_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction;
	// Restart Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* RestartAction;

public:
	// Sets default values for this character's properties
	AZombieCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void CanTurn(const FInputActionValue& Value);
	//void Crouch(const FInputActionValue& Value);
	void RestartLevel();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Base speed
	UPROPERTY(EditAnywhere, Category = "Movement")
		float fBaseSpeed = 5.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float fAcceleration = .01f;

	bool bCanTurn = false;
	FRotator rDesireRotation = FRotator(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category = "Movement")
		float fTurnSpeed = 10.f;

	void Die();
	void ObstacleCollision();
	void PlayFlashEffect(float fMultiplier = 1.f, FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f), float fPlayRate = 5.f);

	// Properties for the timeline to turn right
	UFUNCTION()
		void ControlFlashing();
	UFUNCTION()
		void SetState();
	UPROPERTY(EditAnywhere, Category = "Life")
		UCurveFloat* FlashCurve;
	bool bReadyState;
	float fCurveValue;
	float fDistanceValue;
	float fTimelineValue;
	FTimeline FlashTimeline;

private:

	void MoveForwardConstant(float DeltaTime);
	void TurnCorner(float DeltaTime);

	bool bDied = false;
	float fGlobalMultiplier = 1.f;

	UMaterialInstanceDynamic* DynamicFlashMaterial;
};
