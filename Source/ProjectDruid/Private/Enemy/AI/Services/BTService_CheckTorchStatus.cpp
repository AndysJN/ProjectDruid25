// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Services/BTService_CheckTorchStatus.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PDPlayerCharacter.h"

UBTService_CheckTorchStatus::UBTService_CheckTorchStatus()
{
	NodeName = TEXT("BTService_CheckTorchStatus");
	Interval = 0.5;
}

void UBTService_CheckTorchStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComponent)) return;

	APDPlayerCharacter* PlayerCharacter = Cast<APDPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!IsValid(PlayerCharacter)) return;

	bool bIsTorchActive = PlayerCharacter->IsTorchActive();

	if (!bIsTorchActive && BlackboardComponent->GetValueAsEnum(StateKey.SelectedKeyName) != 0)
	{
		BlackboardComponent->SetValueAsEnum(StateKey.SelectedKeyName, 0);
		BlackboardComponent->ClearValue(AttackTargetKey.SelectedKeyName);
		
	}
}
