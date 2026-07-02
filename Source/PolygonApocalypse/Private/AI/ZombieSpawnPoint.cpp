#include "AI/ZombieSpawnPoint.h"
#include "Components/BillboardComponent.h"

AZombieSpawnPoint::AZombieSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	EditorIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorIcon"));
	RootComponent = EditorIcon;
#endif
}

void AZombieSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}