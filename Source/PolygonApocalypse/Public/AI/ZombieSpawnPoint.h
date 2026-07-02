#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnPoint.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API AZombieSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	AZombieSpawnPoint();

protected:
	virtual void BeginPlay() override;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UBillboardComponent* EditorIcon;
#endif
};