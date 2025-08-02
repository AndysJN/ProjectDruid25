#include "Enemy/PDEnemyBase.h"


APDEnemyBase::APDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APDEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}


