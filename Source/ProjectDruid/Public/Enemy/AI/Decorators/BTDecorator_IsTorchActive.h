// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsTorchActive.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDRUID_API UBTDecorator_IsTorchActive : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsTorchActive();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
