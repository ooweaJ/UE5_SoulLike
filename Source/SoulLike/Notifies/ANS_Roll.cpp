#include "Notifies/ANS_Roll.h"
#include "Actor/Character/Player/BasePlayer.h"

FString UANS_Roll::GetNotifyName_Implementation() const
{
	return "Roll";
}

void UANS_Roll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp == nullptr) return;

	ABasePlayer* Character = Cast<ABasePlayer>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->bRoll = true;
}

void UANS_Roll::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr) return;

	ABasePlayer* Character = Cast<ABasePlayer>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->bRoll = false;
}
