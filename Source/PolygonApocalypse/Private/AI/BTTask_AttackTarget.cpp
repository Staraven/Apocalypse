#include "AI/BTTask_AttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/ZombieCharacter.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName = "Attack Target";
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	AZombieCharacter* Zombie = Cast<AZombieCharacter>(AIController->GetPawn());
	if (!Zombie) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Failed;

	Zombie->AttackTarget(Target);

	return EBTNodeResult::Succeeded;
}