#pragma once

#include "CoreMinimal.h"
#include "PDCharacterBase.h"
#include "Interfaces/Public/Damageable.h"
#include "PDPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTDRUID_API APDPlayerCharacter : public APDCharacterBase, public IDamageable
{
	GENERATED_BODY()

public:
	APDPlayerCharacter();

	virtual void TakeDamage(AActor* DamageCauser, float DamageAmount) override;

protected:
	virtual void BeginPlay() override;

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
};
