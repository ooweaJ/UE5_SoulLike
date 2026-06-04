#include "Behavior/Task_Skill.h"
#include "Actor/Controller/AIController/BaseAIController.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "Component/StateComponent.h"


UTask_Skill::UTask_Skill()
{
    NodeName = "Skill";

    bNotifyTick = true;
}

EBTNodeResult::Type UTask_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	controller->OnSkill(AbilityNum);
	controller->K2_SetFocus(controller->GetTarget());

	return EBTNodeResult::InProgress;
}

void UTask_Skill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (State->IsIdleMode())
	{
		ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
		if (controller)
		{
			controller->K2_ClearFocus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
