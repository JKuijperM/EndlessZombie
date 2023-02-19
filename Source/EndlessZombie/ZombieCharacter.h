// Copyright Jorge Kuijper. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
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

public:
	// Sets default values for this character's properties
	AZombieCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	//void Crouch(const FInputActionValue& Value);

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
		float fAcceleration = .001f; 

	void RotateCharacter();
	bool bStraight = true;
	int iSideTurn = 1;

	// Properties for the timeline to turn right
	UFUNCTION()
		void ControlTurning();
	UFUNCTION()
		void SetState();
	UPROPERTY(EditAnywhere, Category = "Movement")
		UCurveFloat* TurnCurve;
	bool bReadyState;
	float fCurveValue;
	float fDistanceValue;
	float fTimelineValue;
	FTimeline TurnTimeline;
	bool bTurning = false;
	bool bRotate = false;

	TMap<int, FVector> mDirections;
	int iCurrentDirection = 0;
	FVector vCurrentDirection;

	void Die();

private:

	void MoveForwardConstant(float DeltaTime);

	void PlayTurn();

	float fControlTurn = 0.f;
	float fPrevCurvValue = 0.f;

	bool bDied = false;

};
