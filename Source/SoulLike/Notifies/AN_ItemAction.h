#pragma once

#include "CoreMinimal.h"
#include "Notifies/EquippedItemAnimNotify.h"
#include "AN_ItemAction.generated.h"

UCLASS()
class SOULLIKE_API UAN_ItemAction : public UEquippedItemAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	uint32 Num = 1;
};
