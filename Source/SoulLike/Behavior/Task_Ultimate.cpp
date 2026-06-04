#include "Behavior/Task_Ultimate.h"
#include "Actor/Controller/AIController/BaseAIController.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "Component/StateComponent.h"

UTask_Ultimate::UTask_Ultimate()
{
	NodeName = "Ultimate";

	bNotifyTick = true;
}

EBTNodeResult::Type UTask_Ultimate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return EBTNodeResult::Failed;

	AAIBaseCharacter* AIPawn = Cast<AAIBaseCharacter>(controller->GetPawn());
	if (!AIPawn) return EBTNodeResult::Failed;

	UStateComponent* state = AIPawn->GetComponentByClass<UStateComponent>();
	if (!state) return EBTNodeResult::Failed;
	State = state;

	if (!state->IsIdleMode()) return EBTNodeResult::Failed;
	controller->OnUltimate();

	return EBTNodeResult::InProgress;
}

void UTask_Ultimate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (State->IsIdleMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
