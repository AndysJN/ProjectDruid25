#include "Player/PDPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"

APDPlayerController::APDPlayerController()
{
	
}

void APDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APDPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APDPlayerController::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APDPlayerController::OnJumpStarted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APDPlayerController::OnJumpCompleted);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component is not of type UEnhancedInputComponent!"));
	}
}

void APDPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void APDPlayerController::Move(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;


	FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.IsNearlyZero()) return;

	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	if (!FMath::IsNearlyZero(MovementVector.Y))
	{
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (!FMath::IsNearlyZero(MovementVector.X))
	{
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APDPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (LookAxisVector.IsNearlyZero()) return;

	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void APDPlayerController::OnJumpStarted(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;

	ACharacter* PlayerCharacter = Cast<ACharacter>(ControlledPawn);
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Jump();
}

void APDPlayerController::OnJumpCompleted(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;

	ACharacter* PlayerCharacter = Cast<ACharacter>(ControlledPawn);
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->StopJumping();
}
