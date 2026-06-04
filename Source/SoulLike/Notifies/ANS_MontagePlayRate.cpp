#include "Notifies/ANS_MontagePlayRate.h"
#include "Actor/Item/Item.h"
#include "Actor/Character/AI/AIBaseCharacter.h"

FString UANS_MontagePlayRate::GetNotifyName_Implementation() const
{
	return "PlayRate";
}

void UANS_MontagePlayRate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr) return;

	if (AAIBaseCharacter* Ai = Cast<AAIBaseCharacter>(MeshComp->GetOwner()))
	{
		Ai->TryStartTimedMontagePlayRate(PlayRate, TriggerChance);
	}

}

void UANS_MontagePlayRate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr) return;

	if (AAIBaseCharacter* Ai = Cast<AAIBaseCharacter>(MeshComp->GetOwner()))
	{
		Ai->EndTimedMontagePlayRate();
	}

	if (AItem* Item = GetCurrentItem(MeshComp))
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			Item->MontagePlayRate(AnimInstance, 1.0f);
		}
	}
}
