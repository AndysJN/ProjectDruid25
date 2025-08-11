#include "Enemy/PDEnemyBase.h"

#include "Player/PDPlayerCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

APDEnemyBase::APDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APDEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

bool APDEnemyBase::IsPlayerInAttackRange() const
{
	// Get player character
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!IsValid(PlayerCharacter)) return false;

	// Calculate distance
	float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

	// Check if player is within attack range
	return Distance <= AttackRange;
}

void APDEnemyBase::ApplyDamageToPlayer()
{
	// Get AI controller and blackboard
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!IsValid(AIController)) return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!IsValid(Blackboard)) return;

	// Get target from blackboard
	APDPlayerCharacter* Player = Cast<APDPlayerCharacter>(Blackboard->GetValueAsObject("AttackTarget"));
	if (!IsValid(Player)) return;

	Player->TakeDamage(this, Damage);

	UE_LOG(LogTemp, Warning, TEXT("Enemy attacked player! Damage: %f"), Damage);
}