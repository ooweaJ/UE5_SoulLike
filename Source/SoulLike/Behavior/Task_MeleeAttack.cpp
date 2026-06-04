#include "Behavior/Task_MeleeAttack.h"
#include "Actor/Controller/AIController/BaseAIController.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "Component/StateComponent.h"

UTask_MeleeAttack::UTask_MeleeAttack()
{
	NodeName = "Melee";

	bNotifyTick = true;
}

EBTNodeResult::Type UTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return EBTNodeResult::Failed;

	AAIBaseCharacter* AIPawn = Cast<AAIBaseCharacter>(controller->GetPawn());
	if (!AIPawn) return EBTNodeResult::Failed;

	UStateComponent* state = AIPawn->GetComponentByClass<UStateComponent>();
	if (!state) return EBTNodeResult::Failed;
	State = state;

	controller->OnMeleeAttack();

	return EBTNodeResult::InProgress;
}

void UTask_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (State->IsActionMode())
	{
		if (State->IsCanCombo())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

	if (State->IsIdleMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

