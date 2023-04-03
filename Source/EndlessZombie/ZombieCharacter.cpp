// Copyright Jorge Kuijper. All Rights Reserved.

#include "ZombieCharacter.h"
#include "EndlessZombieGameMode.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	bReadyState = true;
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

	DynamicFlashMaterial = GetMesh()->CreateDynamicMaterialInstance(0);

	if (FlashCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFisihedCallback;

		TimelineCallback.BindUFunction(this, FName("ControlFlashing"));
		TimelineFisihedCallback.BindUFunction(this, FName("SetState"));
		FlashTimeline.AddInterpFloat(FlashCurve, TimelineCallback);
		FlashTimeline.SetTimelineFinishedFunc(TimelineFisihedCallback);
	}
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlashTimeline.TickTimeline(DeltaTime);

	if (!bDied)
	{
		MoveForwardConstant(DeltaTime);
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AZombieCharacter::CanTurn);


		// Crouching
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AZombieCharacter::Crouch);
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ACharacter::);
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACharacter::StopCrouching);

		// RestartLevel
		EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Started, this, &AZombieCharacter::RestartLevel);
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

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
void AZombieCharacter::CanTurn(const FInputActionValue& Value)
{
	float fTurnSide = Value.Get<FVector2D>().X;
	if (bCanTurn)
	{
		rDesireRotation = rDesireRotation + FRotator(0.f, fTurnSide * 90.f, 0.f);
		bCanTurn = false;
	}
}

void AZombieCharacter::RestartLevel()
{
	AEndlessZombieGameMode* CurrentGameMode = Cast<AEndlessZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode)
	{
		CurrentGameMode->RestartLevel();
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

void AZombieCharacter::ObstacleCollision()
{
	iPlayerLife--;
	if (iPlayerLife > 0)
	{
		PlayFlashEffect();
	}
	else
	{
		Die();
	}
}

void AZombieCharacter::PlayFlashEffect(float fMultiplier, FLinearColor Color, float fPlayRate)
{
	if (DynamicFlashMaterial)
	{
		fGlobalMultiplier = fMultiplier;
		DynamicFlashMaterial->SetScalarParameterValue("FlashMultiplier", fMultiplier);
		DynamicFlashMaterial->SetVectorParameterValue("FlashColor", Color);
		FlashTimeline.SetPlayRate(fPlayRate);
		FlashTimeline.PlayFromStart();
	}
}

void AZombieCharacter::ControlFlashing()
{
	fTimelineValue = FlashTimeline.GetPlaybackPosition();
	float fFlashCurve = FlashCurve->GetFloatValue(fTimelineValue);
	
	float alpha = FMath::Lerp(0.f, fGlobalMultiplier, fFlashCurve);
	DynamicFlashMaterial->SetScalarParameterValue("FlashMultiplier", alpha);
}

void AZombieCharacter::SetState()
{
	bReadyState = true;
}

void AZombieCharacter::MoveForwardConstant(float DeltaTime)
{
	if (Controller)
	{
		TurnCorner(DeltaTime);
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//UE_LOG(LogTemp, Warning, TEXT("%f"), fBaseSpeed);
		FVector vLocation = GetActorLocation();
		fBaseSpeed += fAcceleration * DeltaTime;
		vLocation += GetActorForwardVector() * fBaseSpeed;
		SetActorLocation(vLocation);
		//AddMovementInput(ForwardDirection, .5f, true);
	}
}

void AZombieCharacter::TurnCorner(float DeltaTime)
{
	if (GetControlRotation() != rDesireRotation)
	{
		FRotator rNewRotation = FMath::RInterpTo(GetControlRotation(), rDesireRotation, DeltaTime, fTurnSpeed);
		Controller->SetControlRotation(rNewRotation);
	}
}