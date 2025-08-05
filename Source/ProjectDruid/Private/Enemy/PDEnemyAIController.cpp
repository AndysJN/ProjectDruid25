#include "Enemy/PDEnemyAIController.h"

APDEnemyAIController::APDEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APDEnemyAIController::MoveToPlayer(AActor* Player)
{
	if (!Player) return;

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	FVector Direction = Player->GetActorLocation() - ControlledPawn->GetActorLocation();
	Direction.Z = 0.f;
	Direction.Normalize();

	FRotator TargetRotation = Direction.Rotation();
	ControlledPawn->SetActorRotation(TargetRotation);

	MoveToActor(Player, 5.0f);
}

void APDEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}
