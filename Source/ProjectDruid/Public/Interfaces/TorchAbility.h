#pragma once

#include "UObject/Interface.h"
#include "TorchAbility.generated.h"


class APDPlayerCharacter;

UINTERFACE(MinimalAPI)
class UTorchAbility : public UInterface
{
	GENERATED_BODY()
};

class PROJECTDRUID_API ITorchAbility
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual bool IsTorchActive() const { return false; }

	UFUNCTION()
	virtual float GetTorchRadius() const { return 0.0f; }

	UFUNCTION()
	virtual void OnTorchStateChanged(bool bActive, APDPlayerCharacter* SourcePlayer = nullptr)
	{
	}
};
