#pragma once

#include "CoreMinimal.h"
#include "PDCharacterBase.h"
#include "Interfaces/Damageable.h"
#include "Interfaces/TorchAbility.h"
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

	// ITorchAbility interface
	virtual bool IsTorchActive() const override;
	virtual float GetTorchRadius() const override;
	virtual void OnTorchStateChanged(bool bActive, APDPlayerCharacter* SourcePlayer = nullptr) override;
	
	UFUNCTION(BlueprintCallable, Category = "Torch")
	void SetTorchAbilityEnabled(bool bEnabled);

protected:
	virtual void BeginPlay() override;

	void ActivateTorch();
	void DeactivateTorch();
	void OnTorchCooldownFinished();
	void ConsumeTorchFuel();

	/** Torch Ability */
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
	float TorchFuelConsumptionRate = 1.0f;

	FTimerHandle TorchActiveTimerHandle;
	FTimerHandle TorchCooldownTimerHandle;
	FTimerHandle TorchFuelTimerHandle;

public:
	/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> PDCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> PDSpringArmComponent;

	/** Character Stats */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Torch")
	void TryActivateTorch();

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> TorchPPMaterialInstance = nullptr;
	UPROPERTY()
	TObjectPtr<APostProcessVolume> TorchPostProcessVolume = nullptr;

	/** Torch Transition */
	UPROPERTY(EditAnywhere, Category = "Torch Transition")
	float CurrentTorchRadius = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Torch Transition")
	float TargetTorchRadius = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Torch Transition")
	float TorchTransitionSpeed = 4.0f;

	FTimerHandle TorchMaterialUpdateTimerHandle;
	void UpdateTorchMaterialParams();
	void StartTorchMaterialUpdate();
	void StopTorchMaterialUpdate();
	void UpdateTorchAbilityState();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
