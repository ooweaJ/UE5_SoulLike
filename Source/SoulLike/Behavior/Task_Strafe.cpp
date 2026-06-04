#include "Behavior/Task_Strafe.h"
#include "Actor/Character/AI/AIBaseCharacter.h"
#include "Actor/Controller/AIController/BaseAIController.h"
#include "Component/StateComponent.h"
#include "Component/StatusComponent.h"

UTask_Strafe::UTask_Strafe()
{
	NodeName = "Strafe";
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return EBTNodeResult::Failed;

	AAIBaseCharacter* aiPawn = Cast<AAIBaseCharacter>(controller->GetPawn());
	if (!aiPawn) return EBTNodeResult::Failed;

	UStateComponent* state = aiPawn->GetComponentByClass<UStateComponent>();
	if (!state->IsIdleMode()) return EBTNodeResult::Failed;

	UStatusComponent* status = aiPawn->GetComponentByClass<UStatusComponent>();
	if(!status) return EBTNodeResult::Failed;

	ACharacter* Target = controller->GetTarget();
	status->SetSpeed(EWalkSpeedTpye::Sneak);
	controller->SetFocus(Target);
	state->SetOffOrient();
	aiPawn->SetStrafe(true);
	float RandomValue = FMath::FRand();

	MotionTime = 0.0f;

	if (RandomValue < 0.5f)
	{
		bRight = true;
	}
	else
	{
		bRight = false;
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UTask_Strafe::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return EBTNodeResult::Failed;

	AAIBaseCharacter* aiPawn = Cast<AAIBaseCharacter>(controller->GetPawn());
	if (!aiPawn) return EBTNodeResult::Failed;

	UStateComponent* state = aiPawn->GetComponentByClass<UStateComponent>();
	if (!state->IsIdleMode()) return EBTNodeResult::Failed;

	UStatusComponent* status = aiPawn->GetComponentByClass<UStatusComponent>();
	if (!status) return EBTNodeResult::Failed;

	status->SetSpeed(EWalkSpeedTpye::Walk);
	state->SetOnOrient();
	controller->ClearFocus(EAIFocusPriority::Gameplay);
	aiPawn->SetStrafe(false);
	return EBTNodeResult::Succeeded;
}

void UTask_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ABaseAIController* controller = Cast<ABaseAIController>(OwnerComp.GetOwner());
	if (!controller) return;

	AAIBaseCharacter* aiPawn = Cast<AAIBaseCharacter>(controller->GetPawn());
	if (!aiPawn) return;

	UStateComponent* state = aiPawn->GetComponentByClass<UStateComponent>();
	if (!state->IsIdleMode()) return;

	UStatusComponent* status = aiPawn->GetComponentByClass<UStatusComponent>();
	if (!status) return;

	MotionTime += DeltaSeconds;

	if (bRight)
		Direction = aiPawn->GetActorRightVector();
	else
		Direction = -aiPawn->GetActorRightVector();

	aiPawn->SetMoveDirection(Direction);

	if (MotionTime >= 3.f)
	{
		status->SetSpeed(EWalkSpeedTpye::Walk);
		state->SetOnOrient();
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		aiPawn->SetStrafe(false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
