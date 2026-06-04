#include "Behavior/Task_Speed.h"
#include "Actor/Controller/AIController/BaseAIController.h"
#include "Actor/Character/BaseCharacter.h"

UTask_Speed::UTask_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UTask_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (AAIController* controller = Cast<AAIController>(OwnerComp.GetOwner()))
	{
		ABaseCharacter* aiPawn = Cast<ABaseCharacter>(controller->GetPawn());
		UStatusComponent* Status = aiPawn->GetComponentByClass<UStatusComponent>();
		if (Status)
		{
			Status->SetSpeed(Type);
			return EBTNodeResult::Succeeded;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
