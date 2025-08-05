#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Damageable.h"
#include "Interfaces/TorchAbility.h"
#include "Player/PDCharacterBase.h"
#include "PDEnemyBase.generated.h"

class USphereComponent;
class APDPlayerCharacter;

UCLASS()
class PROJECTDRUID_API APDEnemyBase : public APDCharacterBase, public IDamageable, public ITorchAbility
{
	GENERATED_BODY()

public:
	APDEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void TakeDamage(AActor* DamageCauser, float DamageAmount) override;

	UFUNCTION()
	void TryAttackPlayer(APDPlayerCharacter* Player);

	UFUNCTION()
	void MoveTowardsPlayer();

	UFUNCTION()
	void OnAttackSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                           const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnChaseSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);

	UFUNCTION()
	void OnChaseSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void StartChasing();

	UFUNCTION()
	void StopChasing();

	virtual void OnTorchStateChanged(bool bActive, APDPlayerCharacter* SourcePlayer = nullptr) override;

protected:
	bool bIsChasingPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ChaseRange = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USphereComponent> AttackSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USphereComponent> ChaseSphere;

	FTimerHandle MoveTimerHandle;
	FTimerHandle AttackTimerHandle;
};
