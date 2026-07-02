#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckTarget.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API UBTService_CheckTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckTarget();

	UPROPERTY(EditAnywhere, Category = "AI")
	float SightRadius = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SightAngle = 90.0f;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};