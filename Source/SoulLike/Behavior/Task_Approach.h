#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Approach.generated.h"

UCLASS()
class SOULLIKE_API UTask_Approach : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_Approach();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	AActor* Target;
	UPROPERTY(EditAnywhere)
	float Acceptance = 100.f;
};
