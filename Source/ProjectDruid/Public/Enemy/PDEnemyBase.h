#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PDEnemyBase.generated.h"

UCLASS()
class PROJECTDRUID_API APDEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	APDEnemyBase();

	virtual void BeginPlay() override;
	
	void ApplyDamageToPlayer();

	// Check if player is within attack range
	bool IsPlayerInAttackRange() const;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 150.0f;
};