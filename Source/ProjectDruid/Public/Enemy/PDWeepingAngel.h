#pragma once

#include "CoreMinimal.h"
#include "PDEnemyBase.h"
#include "PDWeepingAngel.generated.h"

UCLASS()
class PROJECTDRUID_API APDWeepingAngel : public APDEnemyBase
{
	GENERATED_BODY()

public:
	APDWeepingAngel();

protected:
	virtual void BeginPlay() override;
};
