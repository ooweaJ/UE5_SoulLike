// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Skill.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UTask_Skill : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_Skill();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	class UStateComponent* State;

	UPROPERTY(Editanywhere)
	uint32 AbilityNum = 1;
};
