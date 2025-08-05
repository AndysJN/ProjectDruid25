#include "Player/PDPlayerCharacter.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/World.h"
#include "Engine/Engine.h"


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
	if (!IsValid(this) || Health <= 0.0f || DamageAmount <= 0.0f) return;

	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

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
			FName CurrentLevel = FName(UGameplayStatics::GetCurrentLevelName(World, true));
			Destroy();
			UGameplayStatics::OpenLevel(World, CurrentLevel);
		}
	}
}

void APDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Find the global post-process volume
	for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
	{
		if (It->bUnbound)
		{
			TorchPostProcessVolume = *It;
			break;
		}
	}

	if (TorchPostProcessVolume && TorchPostProcessVolume->Settings.WeightedBlendables.Array.Num() > 0)
	{
		UMaterialInterface* BaseMat = Cast<UMaterialInterface>(
			TorchPostProcessVolume->Settings.WeightedBlendables.Array[0].Object);
		if (BaseMat)
		{
			TorchPPMaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
			TorchPostProcessVolume->Settings.WeightedBlendables.Array[0].Object = TorchPPMaterialInstance;
		}
	}

	if (!bTorchAbilityEnabled)
	{
		bTorchActive = true;
	}

	// Initialize the post-process material based on torch ability state
	UpdateTorchAbilityState();
}

void APDPlayerCharacter::UpdateTorchAbilityState()
{
	if (!TorchPPMaterialInstance)
	{
		return;
	}

	if (bTorchAbilityEnabled)
	{
		TorchPPMaterialInstance->SetScalarParameterValue(TEXT("EnableEffect"), 0.0f);

		// Set target radius based on torch state
		TargetTorchRadius = bTorchActive ? TorchRadius : 0.0f;

		if (bTorchActive)
		{
			StartTorchMaterialUpdate();
		}
		else
		{
			// Keep updating while fading out
			StartTorchMaterialUpdate();
		}
	}
	else
	{
		TorchPPMaterialInstance->SetScalarParameterValue(TEXT("EnableEffect"), 1.0f);
		TorchPPMaterialInstance->SetScalarParameterValue(TEXT("PingRadius"), 0.0f);
		TargetTorchRadius = 0.0f;
		CurrentTorchRadius = 0.0f;
		StopTorchMaterialUpdate();
	}
}

void APDPlayerCharacter::SetTorchAbilityEnabled(bool bEnabled)
{
	if (bTorchAbilityEnabled != bEnabled)
	{
		bTorchAbilityEnabled = bEnabled;

		GetWorldTimerManager().ClearTimer(TorchActiveTimerHandle);
		GetWorldTimerManager().ClearTimer(TorchCooldownTimerHandle);
		GetWorldTimerManager().ClearTimer(TorchFuelTimerHandle);

		bTorchActive = false;

		UpdateTorchAbilityState();
	}
}

void APDPlayerCharacter::StartTorchMaterialUpdate()
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	if (!GetWorldTimerManager().IsTimerActive(TorchMaterialUpdateTimerHandle))
	{
		GetWorldTimerManager().SetTimer(TorchMaterialUpdateTimerHandle, this,
		                                &APDPlayerCharacter::UpdateTorchMaterialParams, DeltaTime, true);
	}
}

void APDPlayerCharacter::StopTorchMaterialUpdate()
{
	// Only stop the update when the radius has reached 0 if we're fading out
	if (!bTorchActive && CurrentTorchRadius <= 0.01f)
	{
		if (GetWorldTimerManager().IsTimerActive(TorchMaterialUpdateTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(TorchMaterialUpdateTimerHandle);
		}
	}
}

void APDPlayerCharacter::UpdateTorchMaterialParams()
{
	if (!TorchPPMaterialInstance || !bTorchAbilityEnabled)
	{
		return;
	}

	// Set target radius based on torch state
	TargetTorchRadius = bTorchActive ? TorchRadius : 0.0f;

	// Interpolate current radius toward target
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	CurrentTorchRadius = FMath::FInterpTo(CurrentTorchRadius, TargetTorchRadius, DeltaTime, TorchTransitionSpeed);

	FVector2D ScreenPos;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && UGameplayStatics::ProjectWorldToScreen(PC, GetActorLocation(), ScreenPos))
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		FVector2D UV = FVector2D(ScreenPos.X / ViewportSize.X, ScreenPos.Y / ViewportSize.Y);
		TorchPPMaterialInstance->SetVectorParameterValue(TEXT("PingCenter"), FLinearColor(UV.X, UV.Y, 0, 0));

		float NormalizedRadius = CurrentTorchRadius / (ViewportSize.X * 2.0f);
		TorchPPMaterialInstance->SetScalarParameterValue(TEXT("PingRadius"), NormalizedRadius);
	}

	// Check if we should stop updating when fading out is complete
	if (!bTorchActive && CurrentTorchRadius <= 0.01f)
	{
		CurrentTorchRadius = 0.0f;
		TorchPPMaterialInstance->SetScalarParameterValue(TEXT("PingRadius"), 0.0f);
		StopTorchMaterialUpdate();
	}
}

void APDPlayerCharacter::OnTorchStateChanged(bool bActive, APDPlayerCharacter* SourcePlayer)
{
	if (!bTorchAbilityEnabled)
	{
		return;
	}

	if (bActive)
	{
		StartTorchMaterialUpdate();

		for (TActorIterator<AActor> It(GetWorld()); It; ++It)
		{
			if (*It != this && It->Implements<UTorchAbility>())
			{
				ITorchAbility* TorchAbility = Cast<ITorchAbility>(*It);
				if (TorchAbility)
				{
					// Pass this player as the source to let the enemy know
					TorchAbility->OnTorchStateChanged(true, this);
				}
			}
		}
	}
	else
	{
		UpdateTorchMaterialParams();
	}
}

void APDPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TorchPPMaterialInstance)
	{
		TorchPPMaterialInstance->SetScalarParameterValue(TEXT("EnableEffect"), 0.0f);
	}
	StopTorchMaterialUpdate();
	Super::EndPlay(EndPlayReason);
}

void APDPlayerCharacter::TryActivateTorch()
{
	if (!bTorchAbilityEnabled || bTorchActive || GetWorldTimerManager().IsTimerActive(TorchCooldownTimerHandle) ||
		TorchFuel <= 0.0f)
	{
		return;
	}
	ActivateTorch();
}

void APDPlayerCharacter::ActivateTorch()
{
	bTorchActive = true;
	OnTorchStateChanged(true);
	GetWorldTimerManager().SetTimer(TorchActiveTimerHandle, this, &APDPlayerCharacter::DeactivateTorch, TorchActiveTime,
	                                false);
	GetWorldTimerManager().SetTimer(TorchFuelTimerHandle, this, &APDPlayerCharacter::ConsumeTorchFuel, 1.0f, true);
}

void APDPlayerCharacter::DeactivateTorch()
{
	if (!bTorchActive)
	{
		return;
	}
	bTorchActive = false;
	OnTorchStateChanged(false);
	GetWorldTimerManager().ClearTimer(TorchActiveTimerHandle);
	GetWorldTimerManager().ClearTimer(TorchFuelTimerHandle);
	GetWorldTimerManager().SetTimer(TorchCooldownTimerHandle, this, &APDPlayerCharacter::OnTorchCooldownFinished,
	                                TorchCooldown, false);
}

void APDPlayerCharacter::OnTorchCooldownFinished()
{
	// Cooldown finished, torch can be activated again
}

void APDPlayerCharacter::ConsumeTorchFuel()
{
	TorchFuel -= TorchFuelConsumptionRate;
	if (TorchFuel <= 0.0f)
	{
		TorchFuel = 0.0f;
		DeactivateTorch();
	}
}

bool APDPlayerCharacter::IsTorchActive() const
{
	if (!bTorchAbilityEnabled)
	{
		return false;
	}
	return bTorchActive;
}

float APDPlayerCharacter::GetTorchRadius() const
{
	return TorchRadius;
}
