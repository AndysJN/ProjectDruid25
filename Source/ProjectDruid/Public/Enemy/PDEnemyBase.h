#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Public/Damageable.h"
#include "Player/PDCharacterBase.h"
#include "PDEnemyBase.generated.h"

class APDPlayerCharacter;

UCLASS()
class PROJECTDRUID_API APDEnemyBase : public APDCharacterBase, public IDamageable
{
	GENERATED_BODY()

public:
	APDEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void TakeDamage(AActor* DamageCauser, float DamageAmount) override;
	
	void TryAttackPlayer(APDPlayerCharacter* Player);
	void CheckAndAttackPlayer();

public:
	/** Enemy Stats */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackInterval = 1.0f;

	FTimerHandle AttackTimerHandle;
};
