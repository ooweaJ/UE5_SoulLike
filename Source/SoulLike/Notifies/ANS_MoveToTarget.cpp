#include "Notifies/ANS_MoveToTarget.h"
#include "GameFramework/Character.h"
#include "Actor/Controller/AIController/BaseAIController.h"

FString UANS_MoveToTarget::GetNotifyName_Implementation() const
{
    return "MoveTo";
}

void UANS_MoveToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        if (ABaseAIController* AIController = Cast<ABaseAIController>(OwnerCharacter->GetController()))
        {
            APawn* TargetPawn = AIController->GetTarget();
            if (TargetPawn)
            {
                // Calculate direction from AI to target
                FVector DirectionToTarget = (TargetPawn->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
                // Calculate offset target location in front of the target
                TargetLocationOffset = TargetPawn->GetActorLocation() - DirectionToTarget * 50.0f;

                StartLocation = OwnerCharacter->GetActorLocation();
                MoveDuration = TotalDuration;
                ElapsedTime = 0.0f;
            }
        }
    }
}

void UANS_MoveToTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        ElapsedTime += FrameDeltaTime;
        float Alpha = FMath::Clamp(ElapsedTime / MoveDuration, 0.0f, 1.0f);
        FVector NewLocation = FMath::Lerp(StartLocation, TargetLocationOffset, Alpha);
        OwnerCharacter->SetActorLocation(NewLocation);
    }
}

void UANS_MoveToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        OwnerCharacter->SetActorLocation(TargetLocationOffset);
    }
}

