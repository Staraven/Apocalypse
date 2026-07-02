#include "Core/WaveManager.h"
#include "AI/ZombieCharacter.h"
#include "AI/ZombieSpawnPoint.h"
#include "Kismet/GameplayStatics.h"

AWaveManager::AWaveManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWaveManager::BeginPlay()
{
	Super::BeginPlay();
}

void AWaveManager::StartWave()
{
	if (bWaveInProgress)
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveManager: StartWave ignored because a wave is already active."));
		return;
	}

	if (ZombieTypes.Num() == 0 || SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveManager: Cannot start wave. ZombieTypes or SpawnPoints is empty."));
		return;
	}

	ZombiesToSpawn = BaseZombieCount + (ZombieIncreasePerDay * (CurrentDay - 1));
	ZombiesSpawnedSoFar = 0;
	bWaveInProgress = true;

	UE_LOG(LogTemp, Warning, TEXT("WaveManager: Wave %d started. Spawning %d zombies."), CurrentDay, ZombiesToSpawn);

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AWaveManager::SpawnSingleZombie, SpawnInterval, true);
	GetWorldTimerManager().SetTimer(WaveCheckTimerHandle, this, &AWaveManager::CheckWaveStatus, 1.0f, true);
}

void AWaveManager::StopWaveManager()
{
	bWaveInProgress = false;

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveCheckTimerHandle);
	GetWorldTimerManager().ClearTimer(NextWaveTimerHandle);
}

void AWaveManager::SpawnSingleZombie()
{
	if (ZombiesSpawnedSoFar >= ZombiesToSpawn)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	int32 RandomSpawnIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
	AZombieSpawnPoint* ChosenSpawn = SpawnPoints[RandomSpawnIndex];
	if (!ChosenSpawn) return;

	int32 RandomTypeIndex = FMath::RandRange(0, ZombieTypes.Num() - 1);
	TSubclassOf<AZombieCharacter> ChosenType = ZombieTypes[RandomTypeIndex];
	if (!ChosenType) return;

	FActorSpawnParameters SpawnParams;
	AZombieCharacter* NewZombie = GetWorld()->SpawnActor<AZombieCharacter>(
		ChosenType, ChosenSpawn->GetActorLocation(), ChosenSpawn->GetActorRotation(), SpawnParams
	);

	if (NewZombie)
	{
		AliveZombies.Add(NewZombie);
	}

	ZombiesSpawnedSoFar++;
}

int32 AWaveManager::GetAliveZombieCount() const
{
	int32 Count = 0;
	for (AZombieCharacter* Zombie : AliveZombies)
	{
		if (Zombie && !Zombie->IsDead())
		{
			Count++;
		}
	}
	return Count;
}

void AWaveManager::CheckWaveStatus()
{
	if (!bWaveInProgress) return;

	if (ZombiesSpawnedSoFar < ZombiesToSpawn) return;

	if (GetAliveZombieCount() <= 0)
	{
		bWaveInProgress = false;
		GetWorldTimerManager().ClearTimer(WaveCheckTimerHandle);
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

		UE_LOG(LogTemp, Warning, TEXT("WaveManager: Wave %d completed."), CurrentDay);

		OnWaveCompleted.Broadcast(CurrentDay);

		// Wave progression is controlled by ZoneManager.
		// StartNextWaveCountdown();
	}
}

void AWaveManager::StartNextWaveCountdown()
{
	GetWorldTimerManager().SetTimer(NextWaveTimerHandle, [this]()
	{
		CurrentDay++;
		StartWave();
	}, TimeBetweenWaves, false);
}

void AWaveManager::ClearDeadZombies()
{
	for (AZombieCharacter* Zombie : AliveZombies)
	{
		if (Zombie && Zombie->IsDead())
		{
			Zombie->Destroy();
		}
	}
	AliveZombies.Empty();
}