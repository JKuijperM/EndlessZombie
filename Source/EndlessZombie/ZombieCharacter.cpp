// Copyright Jorge Kuijper. All Rights Reserved.


#include "ZombieCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetWorldLocation(FVector(0.f, 0.f, 180.f));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetWorldRotation(FQuat(FRotator(-15.f, 0.f, 0.f)));

	// For timeline
	bReadyState = true;

	mDirections.Add(0, FVector(1, 0, 0));
	mDirections.Add(1, FVector(0, 1, 0));
	mDirections.Add(2, FVector(-1, 0, 0));
	mDirections.Add(3, FVector(0, -1, 0));

	vCurrentDirection = mDirections[0];
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//CurrentRotation = GetActorRotation();

	if (TurnCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFisihedCallback;

		TimelineCallback.BindUFunction(this, FName("ControlTurning"));
		TimelineFisihedCallback.BindUFunction(this, FName("SetState"));
		TurnTimeline.AddInterpFloat(TurnCurve, TimelineCallback);
		TurnTimeline.SetTimelineFinishedFunc(TimelineFisihedCallback);
	}
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDied)
	{

	TurnTimeline.TickTimeline(DeltaTime);

	if (!bTurning)
	{
		MoveForwardConstant(DeltaTime);
	}
	}
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZombieCharacter::Move);

		// Crouching
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AZombieCharacter::Crouch);
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ACharacter::);
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACharacter::StopCrouching);
	}
}

void AZombieCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//void AZombieCharacter::Crouch(const FInputActionValue& Value)
//{
//
//}

void AZombieCharacter::Die()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	SetActorEnableCollision(true);

	// Ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}

	bDied = true;
}

void AZombieCharacter::MoveForwardConstant(float DeltaTime)
{
	if (bStraight)
	{
		FVector vLocation = GetActorLocation();		
		fBaseSpeed += fAcceleration * DeltaTime;
		vLocation += GetActorForwardVector() * fBaseSpeed;
		SetActorLocation(vLocation);
	}
	else
	{
		RotateCharacter();
		bStraight = true;
	}

}

void AZombieCharacter::PlayTurn()
{
	bTurning = true;
	if (fControlTurn < 90.f)
	{
		TurnTimeline.PlayFromStart();
	}
	else
	{
		fControlTurn = 0.f;
	}
}

void AZombieCharacter::RotateCharacter()
{
	FRotator CurrentRotation = GetActorRotation();
	fControlTurn = 0.f;
	PlayTurn();
}

void AZombieCharacter::ControlTurning()
{
	fTimelineValue = TurnTimeline.GetPlaybackPosition();
	float fTurnCurve = TurnCurve->GetFloatValue(fTimelineValue);

	float fVal = fTurnCurve - fPrevCurvValue;
	fPrevCurvValue = fTurnCurve;

	fControlTurn += fVal;

	if (fControlTurn <= 90.f)
	{
		FRotator CurrentRotation = Controller->GetControlRotation();
		CurrentRotation.Yaw += fVal * iSideTurn;
		Controller->SetControlRotation(CurrentRotation);		
	}

}

void AZombieCharacter::SetState()
{
	bReadyState = true;
	bTurning = false;
	bStraight = true;
	fPrevCurvValue = 0.f;
}