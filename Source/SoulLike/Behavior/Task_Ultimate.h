// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Ultimate.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UTask_Ultimate : public UBTTaskNode
{
	GENERATED_BODY()
  
public:
	UTask_Ultimate();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	class UStateComponent* State;

};
