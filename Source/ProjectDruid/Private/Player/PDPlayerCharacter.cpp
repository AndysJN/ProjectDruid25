#include "Player/PDPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


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

void APDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}


