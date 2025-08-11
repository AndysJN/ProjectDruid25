// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckTorchStatus.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDRUID_API UBTService_CheckTorchStatus : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckTorchStatus();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector StateKey;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector AttackTargetKey;
};
