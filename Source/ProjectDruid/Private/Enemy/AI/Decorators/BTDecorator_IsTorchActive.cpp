// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Decorators/BTDecorator_IsTorchActive.h"

#include "Kismet/GameplayStatics.h"
#include "Player/PDPlayerCharacter.h"

UBTDecorator_IsTorchActive::UBTDecorator_IsTorchActive()
{
	NodeName = TEXT("BTDecorator_IsTorchActive");
}

bool UBTDecorator_IsTorchActive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APDPlayerCharacter* PlayerCharacter = Cast<APDPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!IsValid(PlayerCharacter))	return false;

	return PlayerCharacter->IsTorchActive();
}
