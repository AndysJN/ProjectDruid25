#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE()
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTDRUID_API IDamageable
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(AActor* DamageCauser, float DamageAmount) = 0;
};
