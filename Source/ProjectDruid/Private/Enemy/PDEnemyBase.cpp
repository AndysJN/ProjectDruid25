#include "Enemy/PDEnemyBase.h"

#include "Components/SphereComponent.h"
#include "Player/PDPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/PDEnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

constexpr float MoveTickInterval = 0.02f;

APDEnemyBase::APDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetSphereRadius(AttackRange);
	AttackSphere->SetupAttachment(GetRootComponent());
	AttackSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ChaseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ChaseSphere"));
	ChaseSphere->SetSphereRadius(ChaseRange);
	ChaseSphere->SetupAttachment(GetRootComponent());
	ChaseSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void APDEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &APDEnemyBase::OnAttackSphereOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &APDEnemyBase::OnAttackSphereEndOverlap);
	ChaseSphere->OnComponentBeginOverlap.AddDynamic(this, &APDEnemyBase::OnChaseSphereOverlap);
	ChaseSphere->OnComponentEndOverlap.AddDynamic(this, &APDEnemyBase::OnChaseSphereEndOverlap);
}

void APDEnemyBase::TakeDamage(AActor* DamageCauser, float DamageAmount)
{
	// Enemy will not take damage. Might give it an empty body in the interface later to avoid having to implement this function here.
}

void APDEnemyBase::TryAttackPlayer(APDPlayerCharacter* Player)
{
	if (!IsValid(Player)) return;

	StopChasing();
	Player->TakeDamage(this, Damage);
	UE_LOG(LogTemp, Warning, TEXT("Attacking player! Damage: %f"), Damage);
}

void APDEnemyBase::MoveTowardsPlayer()
{
	auto* Player = Cast<APDPlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!IsValid(Player) || !Player->IsTorchActive())
	{
		StopChasing();
		return;
	}

	auto* AIController = Cast<APDEnemyAIController>(GetController());
	if (IsValid(AIController))
	{
		AIController->MoveToPlayer(Player);
	}
}

void APDEnemyBase::StopChasing()
{
	if (!bIsChasingPlayer) return;

	bIsChasingPlayer = false;
	GetWorldTimerManager().ClearTimer(MoveTimerHandle);

	auto* AIController = Cast<APDEnemyAIController>(GetController());
	if (IsValid(AIController))
	{
		AIController->StopMovement();
	}
}

void APDEnemyBase::StartChasing()
{
	if (bIsChasingPlayer) return;

	bIsChasingPlayer = true;
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &APDEnemyBase::MoveTowardsPlayer, MoveTickInterval, true);
}

void APDEnemyBase::OnAttackSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	auto* Player = Cast<APDPlayerCharacter>(OtherActor);
	if (!IsValid(Player)) return;

	if (!GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
	{
		GetWorldTimerManager().SetTimer(AttackTimerHandle, [this, Player]()
		{
			TryAttackPlayer(Player);
		}, AttackInterval, true);
	}
}

void APDEnemyBase::OnAttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto* Player = Cast<APDPlayerCharacter>(OtherActor);
	if (!IsValid(Player)) return;

	GetWorldTimerManager().ClearTimer(AttackTimerHandle);

	if (ChaseSphere && ChaseSphere->IsOverlappingActor(Player) && Player->IsTorchActive())
	{
		StartChasing();
	}
}

void APDEnemyBase::OnChaseSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	auto* Player = Cast<APDPlayerCharacter>(OtherActor);
	if (IsValid(Player) && Player->IsTorchActive())
	{
		StartChasing();
	}
}

void APDEnemyBase::OnChaseSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto* Player = Cast<APDPlayerCharacter>(OtherActor);
	if (IsValid(Player))
	{
		StopChasing();
	}
}

void APDEnemyBase::OnTorchStateChanged(bool bActive, APDPlayerCharacter* SourcePlayer)
{
	if (!bActive)
	{
		StopChasing();
		return;
	}

	APDPlayerCharacter* Player = SourcePlayer;
	if (!Player)
	{
		Player = Cast<APDPlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	}

	if (IsValid(Player) && Player->IsTorchActive() && ChaseSphere && ChaseSphere->IsOverlappingActor(Player))
	{
		StartChasing();
	}
}
