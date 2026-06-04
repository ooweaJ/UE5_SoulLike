// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_AI.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UService_AI : public UBTService
{
	GENERATED_BODY()
public:
	UService_AI();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
