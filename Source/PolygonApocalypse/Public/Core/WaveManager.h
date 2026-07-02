#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveCompleted, int32, CompletedDay);

UCLASS()
class POLYGONAPOCALYPSE_API AWaveManager : public AActor
{
	GENERATED_BODY()

public:
	AWaveManager();

	// Zombie classes that can be spawned by this wave manager.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<TSubclassOf<class AZombieCharacter>> ZombieTypes;

	// Spawn points placed manually in the level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<class AZombieSpawnPoint*> SpawnPoints;

	// Current day/wave number.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentDay = 1;

	// Base number of zombies spawned in the first wave.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 BaseZombieCount = 5;

	// Additional zombies added per day.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 ZombieIncreasePerDay = 3;

	// Delay between individual zombie spawns.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float SpawnInterval = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartWave();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	int32 GetAliveZombieCount() const;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveCompleted OnWaveCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float TimeBetweenWaves = 5.0f;

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void CheckWaveStatus();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StopWaveManager();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void ClearDeadZombies();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnSingleZombie();
	void StartNextWaveCountdown();

	int32 ZombiesToSpawn = 0;
	int32 ZombiesSpawnedSoFar = 0;
	FTimerHandle SpawnTimerHandle;

	bool bWaveInProgress = false;
	FTimerHandle WaveCheckTimerHandle;
	FTimerHandle NextWaveTimerHandle;

	UPROPERTY()
	TArray<AZombieCharacter*> AliveZombies;
};