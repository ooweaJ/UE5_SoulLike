#include "Behavior/Service_AI.h"
#include "Actor/Controller/AIController/BaseAIController.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "Component/BehaviorComponent.h"
#include "Component/StateComponent.h"

UService_AI::UService_AI()
{
	NodeName = "AI";
}

void UService_AI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return;

	UBehaviorComponent* behavior = controller->GetComponentByClass<UBehaviorComponent>();
	if (!behavior) return;

	AAIBaseCharacter* aiPawn = Cast<AAIBaseCharacter>(controller->GetPawn());

	UStateComponent* State = aiPawn->GetComponentByClass<UStateComponent>();
	if (!State) return;

	if (State->IsDeadMode())
	{
		behavior->SetDeadMode();
		return;
	}

	ACharacter* Target = behavior->GetTarget();

	if (Target == nullptr)
	{
		behavior->SetWaitMode();
		return;
	}
	else
	{
		float distance = aiPawn->GetDistanceTo(Target);

		if (controller->GetAttackRange() > distance)
		{
			behavior->SetMeleeMode();
			return;
		}
		else
		{
			behavior->SetApproachMode();
			return;
		}

	}
}
