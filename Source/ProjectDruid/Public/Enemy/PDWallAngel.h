#pragma once

#include "CoreMinimal.h"
#include "PDEnemyBase.h"
#include "PDWallAngel.generated.h"

UCLASS()
class PROJECTDRUID_API APDWallAngel : public APDEnemyBase
{
	GENERATED_BODY()

public:
	APDWallAngel();

protected:
	virtual void BeginPlay() override;
};
