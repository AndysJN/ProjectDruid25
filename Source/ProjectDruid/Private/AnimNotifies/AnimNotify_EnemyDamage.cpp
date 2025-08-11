#include "AnimNotifies/AnimNotify_EnemyDamage.h"
#include "Enemy/PDEnemyBase.h"

void UAnimNotify_EnemyDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
		return;

	APDEnemyBase* Enemy = Cast<APDEnemyBase>(MeshComp->GetOwner());
	if (Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy damage notify triggered!"));
		if (Enemy->IsPlayerInAttackRange())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player in range - applying damage"));
			Enemy->ApplyDamageToPlayer();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player out of range - no damage applied"));
		}
	}
}