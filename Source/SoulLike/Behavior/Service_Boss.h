#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_Boss.generated.h"

UCLASS()
class SOULLIKE_API UService_Boss : public UBTService
{
	GENERATED_BODY()
public:
	UService_Boss();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
