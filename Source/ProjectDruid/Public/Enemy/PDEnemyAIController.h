#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PDEnemyAIController.generated.h"

UCLASS()
class PROJECTDRUID_API APDEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	APDEnemyAIController();
	void MoveToPlayer(AActor* Player);

protected:
	virtual void BeginPlay() override;
};
