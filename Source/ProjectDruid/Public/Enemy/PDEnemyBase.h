#pragma once

#include "CoreMinimal.h"
#include "Player/PDCharacterBase.h"
#include "PDEnemyBase.generated.h"

UCLASS()
class PROJECTDRUID_API APDEnemyBase : public APDCharacterBase
{
	GENERATED_BODY()

public:
	APDEnemyBase();

protected:
	virtual void BeginPlay() override;
};
