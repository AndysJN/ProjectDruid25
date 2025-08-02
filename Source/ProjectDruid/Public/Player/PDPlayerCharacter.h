#pragma once

#include "CoreMinimal.h"
#include "PDCharacterBase.h"
#include "PDPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTDRUID_API APDPlayerCharacter : public APDCharacterBase
{
	GENERATED_BODY()

public:
	APDPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> PDCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> PDSpringArmComponent;
};
