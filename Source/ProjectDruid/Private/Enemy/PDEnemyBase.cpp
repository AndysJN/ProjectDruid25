#include "Enemy/PDEnemyBase.h"

#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "Player/PDPlayerCharacter.h"


APDEnemyBase::APDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APDEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APDEnemyBase::CheckAndAttackPlayer, AttackInterval, true);
}

void APDEnemyBase::CheckAndAttackPlayer()
{
	// TODO: Implement a more sophisticated player detection system, once the Enemy AI is implemented.
	for (TActorIterator<APDPlayerCharacter> It(GetWorld()); It; ++It)
	{
		APDPlayerCharacter* Player = *It;
		if (IsValid(Player))
		{
			TryAttackPlayer(Player);
			break;
		}
	}
}

void APDEnemyBase::TakeDamage(AActor* DamageCauser, float DamageAmount)
{
	/** Player and/or other actors won't be dealing damage to the enemies for now*/
}

void APDEnemyBase::TryAttackPlayer(APDPlayerCharacter* Player)
{
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		AttackRange,
		24, 
		FColor::Red,
		true,
		-1.0f, 
		0,     
		2.0f
	);
	
	if (!IsValid(this) || !IsValid(Player)) return;
	
	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("Distance to player: %f"), Distance);

	// TODO: Implement a more sophisticated attack system, once the Enemy animation is implemented. This should be in sync with animations.
	if (Distance > AttackRange) return;
	Player->TakeDamage(this, Damage);
}
