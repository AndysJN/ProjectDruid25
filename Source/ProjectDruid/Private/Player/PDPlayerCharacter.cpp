#include "Player/PDPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


APDPlayerCharacter::APDPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	/** Default Values */
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	PDSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	PDSpringArmComponent->SetupAttachment(GetRootComponent());
	PDSpringArmComponent->TargetArmLength = 400.0f;
	PDSpringArmComponent->bUsePawnControlRotation = true;

	PDCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PDCameraComponent->SetupAttachment(PDSpringArmComponent);
}

void APDPlayerCharacter::TakeDamage(AActor* DamageCauser, float DamageAmount)
{
	if (!IsValid(this) || Health <= 0.0f || DamageAmount <= 0.0f)
	{
		return;
	}
	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0, MaxHealth);
	
	UE_LOG(LogTemp, Warning, TEXT("Player took %f damage, Health: %f"), DamageAmount, Health);
	if (Health <= 0.0f)
	{
		/** Disable Input When Health is 0 */
		AController* PlayerController = GetController();
		if (IsValid(PlayerController))
		{
			PlayerController->DisableInput(nullptr);
		}

		/** PLACEHOLDER: Destroy the player character and restart the level */
		/* TODO: Handle player death more gracefully. Restart the player from latest checkpoint etc.
		 * when Save and Load system is implemented.
		 */
		UWorld* World = GetWorld();
		if (World)
		{
			FName CurrentLevel = *World->GetMapName();
			Destroy();
			UGameplayStatics::OpenLevel(World, CurrentLevel);
		}
	}
}

void APDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}
