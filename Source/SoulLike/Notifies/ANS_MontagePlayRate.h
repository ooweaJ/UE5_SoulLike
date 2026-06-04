// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Notifies/EquippedItemAnimNotifyState.h"
#include "ANS_MontagePlayRate.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UANS_MontagePlayRate : public UEquippedItemAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	float PlayRate = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Montage")
	float TriggerChance = 0.5f;
};
