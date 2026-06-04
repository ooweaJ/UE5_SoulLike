#include "Notifies/ANS_Parrying.h"
#include "Actor/Character/BaseCharacter.h"
FString UANS_Parrying::GetNotifyName_Implementation() const
{
	return "Parry";
}

void UANS_Parrying::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp == nullptr) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->SetParrying(true);
}

void UANS_Parrying::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->SetParrying(false);
}
