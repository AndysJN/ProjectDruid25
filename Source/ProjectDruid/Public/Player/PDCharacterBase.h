#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PDCharacterBase.generated.h"


UCLASS(Abstract)
class PROJECTDRUID_API APDCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APDCharacterBase();

protected:
	virtual void BeginPlay() override;
};
