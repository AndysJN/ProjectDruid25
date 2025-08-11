#pragma once

#include "CoreMinimal.h"
#include "PDCharacterBase.h"
#include "Interfaces/Damageable.h"
#include "Interfaces/TorchAbility.h"
#include "Sound/SoundBase.h"
#include "PDPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMaterialInstanceDynamic;
class APostProcessVolume;

UCLASS()
class PROJECTDRUID_API APDPlayerCharacter : public APDCharacterBase, public IDamageable, public ITorchAbility
{
	GENERATED_BODY()

public:
	APDPlayerCharacter();
	
	virtual void TakeDamage(AActor* DamageCauser, float DamageAmount) override;
	
	virtual bool IsTorchActive() const override;
	virtual float GetTorchRadius() const override;
	virtual void OnTorchStateChanged(bool bActive, APDPlayerCharacter* SourcePlayer = nullptr) override;
	
	UFUNCTION(BlueprintCallable, Category = "Torch")
	void SetTorchAbilityEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Torch")
	void TryActivateTorch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> PDCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> PDSpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void ActivateTorch();
	void DeactivateTorch();
	void OnTorchCooldownFinished();
	void ConsumeTorchFuel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	bool bTorchActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	bool bTorchAbilityEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	float TorchRadius = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	float TorchCooldown = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	float TorchActiveTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	float TorchFuel = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	float MaxTorchFuel = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	float TorchFuelConsumptionRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Torch Visuals")
	float TorchFlickerIntensity = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Torch Visuals")
	float TorchFlickerSpeed = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Torch Visuals")
	bool bTorchRespondsToMovement = true;

	// Audio properties for torch activation/deactivation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch Audio")
	TObjectPtr<USoundBase> TorchActivationSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch Audio")
	TObjectPtr<USoundBase> TorchDeactivationSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch Audio")
	TObjectPtr<USoundBase> TorchReadySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch Audio")
	float TorchSoundVolume = 1.0f;

	FTimerHandle TorchActiveTimerHandle;
	FTimerHandle TorchCooldownTimerHandle;
	FTimerHandle TorchFuelTimerHandle;

private:
	void UpdateTorchMaterialParams();
	void StartTorchMaterialUpdate();
	void StopTorchMaterialUpdate();
	void UpdateTorchAbilityState();

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> TorchPPMaterialInstance = nullptr;

	UPROPERTY()
	TObjectPtr<APostProcessVolume> TorchPostProcessVolume = nullptr;

	UPROPERTY(EditAnywhere, Category = "Torch Transition")
	float CurrentTorchRadius = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Torch Transition")
	float TargetTorchRadius = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Torch Transition")
	float TorchTransitionSpeed = 4.0f;

	FTimerHandle TorchMaterialUpdateTimerHandle;
};
