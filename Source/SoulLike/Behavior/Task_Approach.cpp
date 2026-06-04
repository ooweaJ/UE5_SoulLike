#include "Behavior/Task_Approach.h"
#include "Actor/Controller/AIController/BaseAIController.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "Component/StateComponent.h"
#include "Navigation/PathFollowingComponent.h"

UTask_Approach::UTask_Approach()
{
	NodeName = "Approach";

	bNotifyTick = true;
}

EBTNodeResult::Type UTask_Approach::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return EBTNodeResult::Failed;

	AAIBaseCharacter* AIPawn = Cast<AAIBaseCharacter>(controller->GetPawn());
	if (!AIPawn) return EBTNodeResult::Failed;

	UStateComponent* state = AIPawn->GetComponentByClass<UStateComponent>();
	if (!state) return EBTNodeResult::Failed;

	if (!state->IsIdleMode()) return EBTNodeResult::Failed;
	Target = controller->GetTarget();

	if(!Target) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UTask_Approach::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return EBTNodeResult::Failed;
	controller->StopMovement();

	return EBTNodeResult::Succeeded;
}

void UTask_Approach::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return;

	AAIBaseCharacter* AIPawn = Cast<AAIBaseCharacter>(controller->GetPawn());
	if (!AIPawn) return;

	FVector location;
	location = Target->GetActorLocation();

	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, Acceptance, false);

	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
