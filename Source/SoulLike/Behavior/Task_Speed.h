#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Component/StatusComponent.h"
#include "Task_Speed.generated.h"

UCLASS()
class SOULLIKE_API UTask_Speed : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_Speed();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere)
	EWalkSpeedTpye Type;
};
