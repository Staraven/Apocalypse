#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZombieAIController();

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};