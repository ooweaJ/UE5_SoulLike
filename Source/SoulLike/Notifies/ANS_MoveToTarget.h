#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_MoveToTarget.generated.h"

UCLASS()
class SOULLIKE_API UANS_MoveToTarget : public UAnimNotifyState
{
	GENERATED_BODY()
public:
    FString GetNotifyName_Implementation() const override;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
    FVector TargetLocationOffset;
    FVector StartLocation;
    float MoveDuration;
    float ElapsedTime;
};
