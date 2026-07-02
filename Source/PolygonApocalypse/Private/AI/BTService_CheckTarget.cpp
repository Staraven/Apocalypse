#include "AI/BTService_CheckTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UBTService_CheckTarget::UBTService_CheckTarget()
{
	NodeName = "Check Target";
	Interval = 0.5f;
}

void UBTService_CheckTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	if (Distance <= SightRadius)
	{
		BlackboardComp->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	}
	else
	{
		BlackboardComp->ClearValue(GetSelectedBlackboardKey());
	}
}